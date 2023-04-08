#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include <gsl/span>
#include <gsl/span_ext>

#include <libunicode/convert.h>
#include <libunicode/scan.h>

#include "parser_state.h"
#include "parser_action.h"
#include "interface_parser_events.h"

namespace vte::parser
{

    /**
    * Terminal Parser.
    *
    * Highly inspired by: https://vt100.net/emu/dec_ansi_parser
    */
    class Parser
    {
    public:
        using ParseError = std::function<void(std::string const&)>;
        using iterator = uint8_t const*;

        Parser(std::shared_ptr<IParserEvents> eventListener)
            : m_eventListener{ eventListener }
        { }

        /*
        * Parses the input string in UTF-8 encoding and emits VT events while processing.
        * With respect to text, only up to @c EventListener::maxBulkTextSequenceWidth() UTF-32 codepoints will
        * be processed.
        * */
        void parseFragment(gsl::span<char const> data);

        [[nodiscard]] State state() const noexcept { return m_state; }

        [[nodiscard]] char32_t precedingGraphicCharacter() const noexcept { return m_scanState.lastCodepointHint; }

        void printUtf8Byte(char ch);

    private:
        enum class ProcessKind
        {
            /// Processing bulk-text in ground state succeed, keep on going.
            ContinueBulk,
            /// Processing bulk-text failed (various reasons), fallback to finit state machine.
            FallbackToFSM
        };

        std::tuple<ProcessKind, size_t> parseBulkText(char const* begin, char const* end) noexcept;
        void processOnceViaStateMachine(uint8_t ch);

        void handle(ActionClass actionClass, Action action, uint8_t codepoint);

        State m_state = State::Ground;
        std::shared_ptr<IParserEvents> m_eventListener;
        unicode::scan_state m_scanState {};
    };

    /// @returns parsed tuple with OSC code and offset to first data parameter byte.
    template <typename T>
    inline std::pair<int, size_t> extractCodePrefix(T const& data) noexcept
    {
        int code = 0;
        size_t i = 0;

        while (i < data.size() && isdigit(data[i]))
            code = code * 10 + (int) (data[i++] - '0');

        if (i == 0 && !data.empty() && data[0] != ';')
        {
            // such as 'L' is encoded as -'L'
            code = -data[0];
            ++i;
        }

        if (i < data.size() && data[i] == ';')
            ++i;

        return std::pair { code, i };
    }

} // namespace vte::parser
