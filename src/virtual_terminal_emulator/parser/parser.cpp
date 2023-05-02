#include "parser.h"

#include <fmt/format.h>

#include <map>
#include <array>
#include <cassert>
#include <sstream>

#include "../utils/vt_indexed.h"

using namespace vte::parser;

constexpr uint8_t operator"" _b(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

struct ParserTable
{
    //! State transition map from (State, Byte) to (State).
    std::array<std::array<State, 256>, std::numeric_limits<State>::size()> transitions {
        std::array<State, 256> { State::Ground /*XXX or Undefined?*/ }
    };

    //! actions to be invoked upon state entry
    std::array<Action, std::numeric_limits<Action>::size()> entryEvents { Action::Undefined };

    //! actions to be invoked upon state exit
    std::array<Action, std::numeric_limits<Action>::size()> exitEvents { Action::Undefined };

    //! actions to be invoked for a given (State, Byte) pair.
    std::array<std::array<Action, 256>, std::numeric_limits<Action>::size()> events;

    //! Standard state machine tables parsing VT225 to VT525.
    static constexpr ParserTable get();

    // {{{ implementation detail
    struct Range
    {
        uint8_t first;
        uint8_t last;
    };

    constexpr void entry(State state, Action action) noexcept
    {
        entryEvents[static_cast<size_t>(state)] = action;
    }

    constexpr void exit(State state, Action action) noexcept
    {
        exitEvents[static_cast<size_t>(state)] = action;
    }

    // Events
    constexpr void event(State state, Action action, uint8_t input) noexcept
    {
        events[static_cast<size_t>(state)][input] = action;
    }

    constexpr void event(State state, Action action, Range input) noexcept
    {
        for (unsigned ch = input.first; ch <= input.last; ++ch)
            event(state, action, static_cast<uint8_t>(ch));
    }

    template <typename Arg, typename Arg2, typename... Args>
    constexpr void event(State s, Action a, Arg a1, Arg2 a2, Args... more)
    {
        event(s, a, a1);
        event(s, a, a2, more...);
    }

    // Transitions *with* actions
    constexpr void transition(State from, State to, Action action, uint8_t input)
    {
        event(from, action, input);
        transitions[static_cast<size_t>(from)][input] = to;
    }

    constexpr void transition(State from, State to, Action action, Range input)
    {
        event(from, action, input);
        for (unsigned ch = input.first; ch <= input.last; ++ch)
            transitions[static_cast<size_t>(from)][ch] = to;
    }

    // template <typename Arg, typename Arg2, typename... Args>
    // constexpr void transition(State s, State t, Action a, Arg a1, Arg2 a2, Args... more)
    // {
    //     transition(s, t, a, a1);
    //     transition(s, t, a, a2, more...);
    // }

    // Transitions *without* actions
    constexpr void transition(State from, State to, uint8_t input)
    {
        event(from, Action::Ignore, input);
        transitions[static_cast<size_t>(from)][input] = to;
    }

    constexpr void transition(State from, State to, Range input)
    {
        event(from, Action::Ignore, input);
        for (unsigned ch = input.first; ch <= input.last; ++ch)
            transitions[static_cast<size_t>(from)][ch] = to;
    }

    // template <typename Arg, typename Arg2, typename... Args>
    // constexpr void transition(State s, State t, Arg a1, Arg2 a2, Args... more)
    // {
    //     transition(s, t, a1);
    //     transition(s, t, a2, more...);
    // }
};

constexpr ParserTable ParserTable::get()
{
    auto constexpr UnicodeRange = Range { 0x80, 0xFF };

    auto t = ParserTable {};

    // Ground
    t.entry(State::Ground, Action::GroundStart);
    t.event(State::Ground, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::Ground, Action::Print, Range { 0x20_b, 0x7F_b });
    t.event(State::Ground, Action::Print, Range { 0xA0_b, 0xFF_b });
    t.event(State::Ground, Action::Print, UnicodeRange);

    // EscapeIntermediate
    t.event(State::EscapeIntermediate,
            Action::Execute,
            Range { 0x00_b, 0x17_b },
            0x19_b,
            Range { 0x1C_b, 0x1F_b });
    t.event(State::EscapeIntermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.event(State::EscapeIntermediate, Action::Ignore, 0x7F_b);
    t.transition(State::EscapeIntermediate, State::Ground, Action::ESC_Dispatch, Range { 0x30_b, 0x7E_b });

    // Escape
    t.entry(State::Escape, Action::Clear);
    t.event(State::Escape, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::Escape, Action::Ignore, 0x7F_b);
    t.transition(State::Escape, State::IgnoreUntilST, 0x58_b); // SOS (start of string): ESC X
    t.transition(State::Escape, State::PM_String, 0x5E_b);     // PM (private message): ESC ^
    t.transition(State::Escape, State::APC_String, 0x5F_b);    // APC (application program command): ESC _
    t.transition(State::Escape, State::DCS_Entry, 0x50_b);
    t.transition(State::Escape, State::OSC_String, 0x5D_b);
    t.transition(State::Escape, State::CSI_Entry, 0x5B_b);
    t.transition(State::Escape, State::Ground, Action::ESC_Dispatch, Range { 0x30_b, 0x4F_b });
    t.transition(State::Escape, State::Ground, Action::ESC_Dispatch, Range { 0x51_b, 0x57_b });
    t.transition(State::Escape, State::Ground, Action::ESC_Dispatch, 0x59_b);
    t.transition(State::Escape, State::Ground, Action::ESC_Dispatch, 0x5A_b);
    t.transition(State::Escape, State::Ground, Action::Ignore, 0x5C_b); // ST for OSC, DCS, ...
    t.transition(State::Escape, State::Ground, Action::ESC_Dispatch, Range { 0x60_b, 0x7E_b });
    t.transition(State::Escape, State::EscapeIntermediate, Action::Collect, Range { 0x20_b, 0x2F_b });

    // IgnoreUntilST
    t.event(State::IgnoreUntilST, Action::Ignore, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    // t.transition(State::IgnoreUntilST, State::Ground, 0x9C_b);

    // DCS_Entry
    t.entry(State::DCS_Entry, Action::Clear);
    t.event(State::DCS_Entry, Action::Ignore, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::DCS_Entry, Action::Ignore, 0x7F_b);
    t.transition(State::DCS_Entry, State::DCS_Intermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.transition(State::DCS_Entry, State::DCS_Ignore, 0x3A_b);
    t.transition(State::DCS_Entry, State::DCS_Param, Action::Param, Range { 0x30_b, 0x39_b });
    t.transition(State::DCS_Entry, State::DCS_Param, Action::Param, 0x3B_b);
    t.transition(State::DCS_Entry, State::DCS_Param, Action::CollectLeader, Range { 0x3C_b, 0x3F_b });
    t.transition(State::DCS_Entry, State::DCS_PassThrough, Range { 0x40_b, 0x7E_b });

    // DCS_Ignore
    t.event(State::DCS_Ignore,
            Action::Ignore,
            Range { 0x00_b, 0x17_b },
            0x19_b,
            Range { 0x1C_b, 0x1F_b },
            Range { 0x20_b, 0x7F_b });
    t.event(State::DCS_Ignore, Action::Print, Range { 0xA0_b, 0xFF_b });
    t.event(State::DCS_Ignore, Action::Print, UnicodeRange);
    // t.transition(State::DCS_Ignore, State::Ground, 0x9C_b);

    // DCS_Intermediate
    t.event(
        State::DCS_Intermediate, Action::Ignore, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::DCS_Intermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.event(State::DCS_Intermediate, Action::Ignore, 0x7F_b);
    t.transition(State::DCS_Intermediate, State::DCS_PassThrough, Range { 0x40_b, 0x7E_b });

    // DCS_PassThrough
    t.entry(State::DCS_PassThrough, Action::Hook);
    t.event(State::DCS_PassThrough,
            Action::Put,
            Range { 0x00_b, 0x17_b },
            0x19_b,
            Range { 0x1C_b, 0x1F_b },
            Range { 0x20_b, 0x7E_b });
    t.event(State::DCS_PassThrough, Action::Ignore, 0x7F_b);
    t.exit(State::DCS_PassThrough, Action::Unhook);
    // t.transition(State::DCS_PassThrough, State::Ground, 0x9C_b);

    // DCS_Param
    t.event(State::DCS_Param, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::DCS_Param, Action::Param, Range { 0x30_b, 0x39_b }, 0x3B_b);
    t.event(State::DCS_Param, Action::Ignore, 0x7F_b);
    t.transition(State::DCS_Param, State::DCS_Ignore, 0x3A_b);
    t.transition(State::DCS_Param, State::DCS_Ignore, Range { 0x3C_b, 0x3F_b });
    t.transition(State::DCS_Param, State::DCS_Intermediate, Range { 0x20_b, 0x2F_b });
    t.transition(State::DCS_Param, State::DCS_PassThrough, Range { 0x40_b, 0x7E_b });

    // OSC_String
    // (xterm extension to also allow BEL (0x07) as OSC terminator)
    t.entry(State::OSC_String, Action::OSC_Start);
    t.event(State::OSC_String,
            Action::Ignore,
            Range { 0x00_b, 0x06_b },
            Range { 0x08_b, 0x17_b },
            0x19_b,
            Range { 0x1C_b, 0x1F_b });
    t.event(State::OSC_String, Action::OSC_Put, Range { 0x20_b, 0x7F_b });
    t.event(State::OSC_String, Action::OSC_Put, Range { 0xA0_b, 0xFF_b });
    t.event(State::OSC_String, Action::OSC_Put, UnicodeRange);
    t.exit(State::OSC_String, Action::OSC_End);
    // t.transition(State::OSC_String, State::Ground, 0x9C_b);
    t.transition(State::OSC_String, State::Ground, 0x07_b);

    // APC_String
    // APC := ESC _ ... ST
    t.entry(State::APC_String, Action::APC_Start);
    t.event(State::APC_String, Action::APC_Put, Range { 0x20_b, 0x7F_b });
    t.event(State::APC_String, Action::APC_Put, Range { 0xA0_b, 0xFF_b });
    t.event(State::APC_String, Action::APC_Put, UnicodeRange);
    t.exit(State::APC_String, Action::APC_End);
    // t.transition(State::APC_String, State::Ground, 0x9C_b); // ST
    t.transition(State::APC_String, State::Ground, 0x07_b); // BEL

    // PM_String
    // PM := ESC ^ ... ST
    t.entry(State::PM_String, Action::PM_Start);
    t.event(State::PM_String,
            Action::PM_Put,
            Range { 0x00_b, 0x17_b },
            0x19_b,
            Range { 0x1C_b, 0x1F_b },
            Range { 0x20_b, 0x7F_b },
            Range { 0xA0_b, 0xFF_b });
    t.event(State::PM_String, Action::PM_Put, UnicodeRange);
    t.exit(State::PM_String, Action::PM_End);
    // t.transition(State::PM_String, State::Ground, 0x9C_b); // ST
    t.transition(State::PM_String, State::Ground, 0x07_b); // BEL

    // CSI_Entry
    t.entry(State::CSI_Entry, Action::Clear);
    t.event(State::CSI_Entry, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::CSI_Entry, Action::Ignore, 0x7F_b);
    t.transition(State::CSI_Entry, State::Ground, Action::CSI_Dispatch, Range { 0x40_b, 0x7E_b });
    t.transition(State::CSI_Entry, State::CSI_Intermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.transition(State::CSI_Entry, State::CSI_Ignore, 0x3A_b);
    t.transition(State::CSI_Entry, State::CSI_Param, Action::ParamDigit, Range { 0x30_b, 0x39_b });
    t.transition(State::CSI_Entry, State::CSI_Param, Action::ParamSeparator, 0x3B_b);
    t.transition(State::CSI_Entry, State::CSI_Param, Action::CollectLeader, Range { 0x3C_b, 0x3F_b });

    // CSI_Param
    t.event(State::CSI_Param, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::CSI_Param, Action::ParamDigit, Range { 0x30_b, 0x39_b });
    t.event(State::CSI_Param, Action::ParamSubSeparator, 0x3A_b);
    t.event(State::CSI_Param, Action::ParamSeparator, 0x3B_b);
    t.event(State::CSI_Param, Action::Ignore, 0x7F_b);
    t.transition(State::CSI_Param, State::CSI_Ignore, Range { 0x3C_b, 0x3F_b });
    t.transition(State::CSI_Param, State::CSI_Intermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.transition(State::CSI_Param, State::Ground, Action::CSI_Dispatch, Range { 0x40_b, 0x7E_b });

    // CSI_Ignore
    t.event(State::CSI_Ignore, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::CSI_Ignore, Action::Ignore, Range { 0x20_b, 0x3F_b }, 0x7F_b);
    t.transition(State::CSI_Ignore, State::Ground, Range { 0x40_b, 0x7E_b });

    // CSI_Intermediate
    t.event(
        State::CSI_Intermediate, Action::Execute, Range { 0x00_b, 0x17_b }, 0x19_b, Range { 0x1C_b, 0x1F_b });
    t.event(State::CSI_Intermediate, Action::Collect, Range { 0x20_b, 0x2F_b });
    t.event(State::CSI_Intermediate, Action::Ignore, 0x7F_b);
    t.transition(State::CSI_Intermediate, State::CSI_Ignore, Range { 0x30_b, 0x3F_b });
    t.transition(State::CSI_Intermediate, State::Ground, Action::CSI_Dispatch, Range { 0x40_b, 0x7E_b });

    // * -> Ground, ...
    for (State anywhere = std::numeric_limits<State>::min(); anywhere <= std::numeric_limits<State>::max();
         ++anywhere)
    {
        t.transition(anywhere, State::Ground, 0x18_b);
        t.transition(anywhere, State::Ground, 0x1A_b);
        t.transition(anywhere, State::Escape, 0x1B_b);

        // C1 control need special 2-byte treatment due to this Parser
        // being UTF-8.
        // t.transition(anywhere, State::Ground, 0x9C_b);
        // t.transition(anywhere, State::Ground, Range{0x80_b, 0x8F_b});
        // t.transition(anywhere, State::Ground, Range{0x91_b, 0x97_b});
        // t.transition(anywhere, State::DCS_Entry, 0x90_b);     // C1: DCS
        // t.transition(anywhere, State::IgnoreUntilST, 0x98_b); // C1: SOS
        // t.transition(anywhere, State::PM_String, 0x9E_b);     // C1: PM
        // t.transition(anywhere, State::APC_String, 0x9F_b);    // C1: APC
    }

    // TODO: verify the above is correct (programatically as much as possible)
    // TODO: see if we can reduce the preassure on L2 caches (is this even an issue?)

    return t;
}

void Parser::parseFragment(gsl::span<char const> data)
{
    const auto* input = data.data();
    const auto* const end = data.data() + data.size();

    while (input != end)
    {
        auto const [processKind, processedByteCount] = parseBulkText(input, end);
        switch (processKind)
        {
            case ProcessKind::ContinueBulk:
                // clang-format off
                input += processedByteCount;
                break;
                // clang-format on
            case ProcessKind::FallbackToFSM:
                processOnceViaStateMachine(static_cast<uint8_t>(*input++));
                break;
        }
    }
}

void Parser::processOnceViaStateMachine(uint8_t ch)
{
    auto const s = static_cast<size_t>(m_state);
    ParserTable static constexpr table = ParserTable::get();

    if (auto const t = table.transitions[s][static_cast<uint8_t>(ch)]; t != State::Undefined)
    {
        // fmt::print("VTParser: Transitioning from {} to {}", _state, t);
        handle(ActionClass::Leave, table.exitEvents[s], ch);
        handle(ActionClass::Transition, table.events[s][static_cast<size_t>(ch)], ch);
        m_state = t;
        handle(ActionClass::Enter, table.entryEvents[static_cast<size_t>(t)], ch);
    }
    else if (Action const a = table.events[s][ch]; a != Action::Undefined)
        handle(ActionClass::Event, a, ch);
    else
        m_eventListener->error("Parser error: Unknown action for state/input pair.");
}

std::tuple<Parser::ProcessKind, size_t> Parser::parseBulkText(char const* begin, char const* end) noexcept
{
    const auto* input = begin;
    if (m_state != State::Ground)
        return { ProcessKind::FallbackToFSM, 0 };

    auto const maxCharCount = m_eventListener->maxBulkTextSequenceWidth();
    if (!maxCharCount)
        return { ProcessKind::FallbackToFSM, 0 };

    auto const chunk = std::string_view(input, static_cast<size_t>(std::distance(input, end)));
    auto const [cellCount, next, subStart, subEnd] = unicode::scan_text(m_scanState, chunk, maxCharCount);

    if (next == input)
        return { ProcessKind::FallbackToFSM, 0 };

    // We do not test on cellCount>0 because the scan could contain only a ZWJ (zero width
    // joiner), and that would be misleading.

    assert(subStart <= subEnd);
    auto const byteCount = static_cast<size_t>(std::distance(subStart, subEnd));
    if (byteCount == 0)
        return { ProcessKind::FallbackToFSM, 0 };

    assert(cellCount <= maxCharCount);
    assert(subEnd <= chunk.data() + chunk.size());
    assert(next <= chunk.data() + chunk.size());

    auto const text = std::string_view { subStart, byteCount };
    if (m_scanState.utf8.expectedLength == 0)
    {
        if (!text.empty())
        {
            m_eventListener->print(text, cellCount);
        }

        // This optimization is for the `cat`-people.
        // It further optimizes the throughput performance by bypassing
        // the FSM for the `(TEXT LF+)+`-case.
        //
        // As of bench-headless, the performance incrrease is about 50x.
        if (input != end && *input == '\n')
            m_eventListener->execute(*input++);
    }
    else
    {
        // fmt::print("Parser.text: incomplete UTF-8 sequence at end: {}/{}\n",
        //            _scanState.utf8.currentLength,
        //            _scanState.utf8.expectedLength);

        // for (char const ch: text)
        //     printUtf8Byte(ch);
    }

    return { ProcessKind::ContinueBulk, static_cast<size_t>(std::distance(input, next)) };
}

void Parser::printUtf8Byte(char ch)
{
    unicode::ConvertResult const r = unicode::from_utf8(m_scanState.utf8, (uint8_t) ch);
    if (std::holds_alternative<unicode::Incomplete>(r))
        return;

    auto constexpr ReplacementCharacter = char32_t { 0xFFFD };
    auto const codepoint = std::holds_alternative<unicode::Success>(r) ? std::get<unicode::Success>(r).value
                                                                       : ReplacementCharacter;
    m_eventListener->print(codepoint);
    m_scanState.lastCodepointHint = codepoint;
}

void Parser::handle(ActionClass actionClass, Action action, uint8_t codepoint)
{
    (void) actionClass;
    auto const ch = static_cast<char>(codepoint);

    switch (action)
    {
        case Action::GroundStart: m_scanState.lastCodepointHint = 0; break;
        case Action::Clear: m_eventListener->clear(); break;
        case Action::CollectLeader: m_eventListener->collectLeader(ch); break;
        case Action::Collect: m_eventListener->collect(ch); break;
        case Action::Param: m_eventListener->param(ch); break;
        case Action::ParamDigit: m_eventListener->paramDigit(ch); break;
        case Action::ParamSeparator: m_eventListener->paramSeparator(); break;
        case Action::ParamSubSeparator: m_eventListener->paramSubSeparator(); break;
        case Action::Execute: m_eventListener->execute(ch); break;
        case Action::ESC_Dispatch: m_eventListener->dispatchESC(ch); break;
        case Action::CSI_Dispatch: m_eventListener->dispatchCSI(ch); break;
        case Action::Print: printUtf8Byte(ch); break;
        case Action::OSC_Start: m_eventListener->startOSC(); break;
        case Action::OSC_Put: m_eventListener->putOSC(ch); break;
        case Action::OSC_End: m_eventListener->dispatchOSC(); break;
        case Action::Hook: m_eventListener->hook(ch); break;
        case Action::Put: m_eventListener->put(ch); break;
        case Action::Unhook: m_eventListener->unhook(); break;
        case Action::APC_Start: m_eventListener->startAPC(); break;
        case Action::APC_Put: m_eventListener->putAPC(ch); break;
        case Action::APC_End: m_eventListener->dispatchAPC(); break;
        case Action::PM_Start: m_eventListener->startPM(); break;
        case Action::PM_Put: m_eventListener->putPM(ch); break;
        case Action::PM_End: m_eventListener->dispatchPM(); break;
        case Action::Ignore:
        case Action::Undefined: break;
    }
}

void vte::parser::parserTableDot(std::ostream& os)
{
    using Transition = std::pair<State, State>;
    using Range = ParserTable::Range;
    using RangeSet = std::vector<Range>;

    ParserTable const& table = ParserTable::get();
    // (State, Byte) -> State
    auto transitions = std::map<Transition, RangeSet> {};
    for ([[maybe_unused]] auto const&& [sourceState, sourceTransitions] : vte::indexed(table.transitions))
    {
        for (auto const [i, targetState] : vte::indexed(sourceTransitions))
        {
            auto const ch = static_cast<uint8_t>(i);
            if (targetState != State::Undefined)
            {
                // os << fmt::format("({}, 0x{:0X}) -> {}\n", static_cast<State>(sourceState), ch,
                //  targetState);
                auto const t = Transition { static_cast<State>(sourceState), targetState };
                if (!transitions[t].empty() && ch == transitions[t].back().last + 1)
                {
                    transitions[t].back().last++;
                }
                else
                {
                    transitions[t].emplace_back(Range { ch, ch });
                }
            }
        }
    }
    // TODO: isReachableFromAnywhere(targetState) to check if x can be reached from anywhere.

    os << "digraph {\n";
    os << "  node [shape=box];\n";
    os << "  ranksep = 0.75;\n";
    os << "  rankdir = LR;\n";
    os << "  concentrate = true;\n";

    unsigned groundCount = 0;

    for (auto const& t: transitions)
    {
        auto const sourceState = t.first.first;
        auto const targetState = t.first.second;

        if (sourceState == State::Undefined)
        {
            continue;
        }

        auto const targetStateName = targetState == State::Ground && targetState != sourceState
            ? fmt::format("{}_{}", targetState, ++groundCount)
            : fmt::format("{}", targetState);

        // if (isReachableFromAnywhere(targetState))
        //     os << fmt::format("  {} [style=dashed, style=\"rounded, filled\", fillcolor=yellow];\n",
        //     sourceStateName);

        if (targetState == State::Ground && sourceState != State::Ground)
        {
            os << fmt::format("  \"{}\" [style=\"dashed, filled\", fillcolor=gray, label=\"ground\"];\n", targetStateName);
        }

        os << fmt::format(R"(  "{}" -> "{}" )", sourceState, targetStateName);
        os << "[";
        os << "label=\"";
        for (auto const&& [rangeCount, u] : vte::indexed(t.second))
        {
            if (rangeCount)
            {
                os << ", ";
                if (rangeCount % 3 == 0)
                {
                    os << "\\n";
                }
            }
            if (u.first == u.last)
            {
                os << fmt::format("{:02X}", u.first);
            }
            else
            {
                os << fmt::format("{:02X}-{:02X}", u.first, u.last);
            }
        }
        os << "\"";
        os << "]";
        os << ";\n";
    }

    // equal ranks
    os << "  { rank=same; ";
    for (auto const state: { State::CSI_Entry, State::DCS_Entry, State::OSC_String })
    {
        os << fmt::format(R"("{}"; )", state);
    }
    os << "};\n";

    os << "  { rank=same; ";
    for (auto const state: { State::CSI_Param, State::DCS_Param, State::OSC_String })
    {
        os << fmt::format(R"("{}"; )", state);
    }
    os << "};\n";

    os << "}\n";
}
