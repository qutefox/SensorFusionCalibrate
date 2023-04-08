#pragma once

#include <limits>
#include <string>
#include <string_view>

namespace vte::parser
{

    enum class State : uint8_t
    {
        /**
        * Internal state to signal that this state doesn't exist (or hasn't been set).
        */
        Undefined,

        /**
        * This is the initial state of the parser, and the state used to consume all characters
        * other than components of escape and control sequences.
        */
        Ground,

        /**
        * This state is entered whenever the C0 control ESC is received.
        *
        * This will immediately cancel any escape sequence,
        * control sequence or control string in progress.
        * If an escape sequence or control sequence was in progress,
        * “cancel” means that the sequence will have no effect,
        * because the final character that determines the control function
        * (in conjunction with any intermediates) will not have been received.
        * However, the ESC that cancels a control string may occur after
        * the control function has been determined and the following string
        * has had some effect on terminal state.
        *
        * For example, some soft characters may already have been defined.
        * Cancelling a control string does not undo these effects.
        */
        Escape,

        /**
        * This state is entered when an intermediate character arrives in an escape sequence.
        *
        * Escape sequences have no parameters, so the control function to be invoked is determined
        * by the intermediate and final characters.
        */
        EscapeIntermediate,

        /**
        * This state is entered when the control function CSI is recognised, in 7-bit or 8-bit form.
        *
        * This state will only deal with the first character of a control sequence, because
        * the characters 3C-3F can only appear as the first character of a control sequence, if
        * they appear at all.
        */
        CSI_Entry,

        /**
        * This state is entered when a parameter character is recognised in a control sequence.
        *
        * It then recognises other parameter characters until an intermediate or final character
        * appears. Further occurrences of the private-marker characters 3C-3F or the character 3A,
        * which has no standardised meaning, will cause transition to the csi ignore state.
        */
        CSI_Param,

        /**
        * This state is entered when an intermediate character is recognised in a control sequence.
        *
        * It then recognises other intermediate characters until a final character appears. If any
        * more parameter characters appear, this is an error condition which will cause a
        * transition to the csi ignore state.
        */
        CSI_Intermediate,

        /**
        * This state is used to consume remaining characters of a control sequence that is still
        * being recognised, but has already been disregarded as malformed.
        *
        * This state will only exit when a final character is recognised,
        * at which point it transitions to ground state
        * without dispatching the control function. This state may be entered because:
        *
        * 1.) a private-marker character 3C-3F is recognised in any place other than the first
        *     character of the control sequence,
        * 2.) the character 3A appears anywhere, or
        * 3.) a parameter character 30-3F occurs after an intermediate character has been recognised.
        *
        * C0 controls will still be executed while a control sequence is being ignored.
        */
        CSI_Ignore,

        /**
        * This state is entered when the control function DCS is recognised, in 7-bit or 8-bit form.
        *
        * X3.64 doesn’t define any structure for device control strings, but Digital made
        * them appear like control sequences followed by a data string, with a form and length
        * dependent on the control function. This state is only used to recognise the first
        * character of the control string, mirroring the csi entry state.
        *
        * C0 controls other than CAN, SUB and ESC are not executed while recognising the first part
        * of a device control string.
        */
        DCS_Entry,

        /**
        * This state is entered when a parameter character is recognised in a device control
        * string. It then recognises other parameter characters until an intermediate or final
        * character appears. Occurrences of the private-marker characters 3C-3F or the undefined
        * character 3A will cause a transition to the dcs ignore state.
        */
        DCS_Param,

        /**
        * This state is entered when an intermediate character is recognised in a device control
        * string. It then recognises other intermediate characters until a final character appears.
        * If any more parameter characters appear, this is an error condition which will cause a
        * transition to the dcs ignore state.
        */
        DCS_Intermediate,

        /**
        * This state is a shortcut for writing state machines for all possible device control
        * strings into the main parser. When a final character has been recognised in a device
        * control string, this state will establish a channel to a handler for the appropriate
        * control function, and then pass all subsequent characters through to this alternate
        * handler, until the data string is terminated (usually by recognising the ST control
        * function).
        *
        * This state has an exit action so that the control function handler can be informed when
        * the data string has come to an end. This is so that the last soft character in a DECDLD
        * string can be completed when there is no other means of knowing that its definition has
        * ended, for example.
        */
        DCS_PassThrough,

        /**
        * This state is used to consume remaining characters of a device control string that is
        * still being recognised, but has already been disregarded as malformed. This state will
        * only exit when the control function ST is recognised, at which point it transitions to
        * ground state. This state may be entered because:
        *
        * 1.) a private-marker character 3C-3F is recognised in any place other than the first
        *     character of the control string,
        * 2.) the character 3A appears anywhere, or
        * 3.) a parameter character 30-3F occurs after an intermediate character has been recognised.
        *
        * These conditions are only errors in the first part of the control string, until a final
        * character has been recognised. The data string that follows is not checked by this
        * parser.
        */
        DCS_Ignore,

        /**
        * This state is entered when the control function OSC (Operating System Command) is
        * recognised. On entry it prepares an external parser for OSC strings and passes all
        * printable characters to a handler function. C0 controls other than CAN, SUB and ESC are
        * ignored during reception of the control string.
        *
        * The only control functions invoked by OSC strings are DECSIN (Set Icon Name) and DECSWT
        * (Set Window Title), present on the multisession VT520 and VT525 terminals. Earlier
        * terminals treat OSC in the same way as PM and APC, ignoring the entire control string.
        */
        OSC_String,

        /**
        * Application Program Command.
        * ESC _ ... ST
        */
        APC_String,

        /*
        * Private Message
        * ESC ^ ... ST
        *
        * The payload need not to be printable characters.
        */
        PM_String,

        /**
        * The VT500 doesn’t define any function for these control strings, so this state ignores
        * all received characters until the control function ST is recognised.
        */
        IgnoreUntilST,
    };

    constexpr State& operator++(State& s) noexcept
    {
        // NB: We allow to increment one element beyond maximum element (IgnoreUntilST) as sentinel
        // in order to allow easy iteration.
        if (s <= State::IgnoreUntilST)
            s = static_cast<State>(1 + static_cast<uint8_t>(s));

        return s;
    }

    constexpr std::string_view to_string(State state)
    {
        switch (state)
        {
            case State::Undefined: return "Undefined";
            case State::Ground: return "Ground";
            case State::Escape: return "Escape";
            case State::EscapeIntermediate: return "EscapeIntermediate";
            case State::CSI_Entry: return "CSI Entry";
            case State::CSI_Param: return "CSI Param";
            case State::CSI_Intermediate: return "CSI Intermediate";
            case State::CSI_Ignore: return "CSI Ignore";
            case State::DCS_Entry: return "DCS Entry";
            case State::DCS_Param: return "DCS Param";
            case State::DCS_Intermediate: return "DCS Intermediate";
            case State::DCS_PassThrough: return "DCS PassThrough";
            case State::DCS_Ignore: return "DCS Ignore";
            case State::OSC_String: return "OSC String";
            case State::APC_String: return "APC String";
            case State::PM_String: return "PM String";
            case State::IgnoreUntilST: return "Ignore Until ST (SOS)";
        }
        return "?";
    }

} // namespace vte::parser

namespace std
{

    template <>
    struct numeric_limits<vte::parser::State>
    {
        using State = vte::parser::State;
        constexpr static State min() noexcept { return State::Ground; } // skip Undefined
        constexpr static State max() noexcept { return State::IgnoreUntilST; }
        constexpr static size_t size() noexcept { return 17; }
    };

} // namespace std
