#pragma once

#include <limits>
#include <string_view>

namespace vte::parser
{

    /**
    * Interface of all events that can be emitted by the Parser.
    */
    class IParserEvents
    {
    public:
        virtual ~IParserEvents() = default;

        /**
        * Invoked on parsing errors (UTF-8 decoding error or parser state errors).
        */
        virtual void error(std::string_view const& errorString) = 0;

        /**
        * This action only occurs in ground state. The current code should be mapped to a glyph
        * according to the character set mappings and shift states in effect, and that glyph should
        * be displayed. 20 (SP) and 7F (DEL) have special behaviour in later VT series, as
        * described in ground.
        */
        virtual void print(char32_t text) = 0;

        /**
        * Optimization that passes in ASCII chars between [0x20 .. 0x7F].
        *
        * @param cellCount reflects the sum of the East Asian Width attribute for all passed codepoints.
        */
        virtual size_t print(std::string_view chars, size_t cellCount) = 0;

        /**
        * Returns the number of terminal columns (cells) that are still available in the current line
        * until the right page margin would be hit.
        *
        * This accessor is used to determine whether or not bulk text processing can
        * be used or not.
        */
        [[nodiscard]] virtual size_t maxBulkTextSequenceWidth() const noexcept = 0;

        /**
        * The C0 or C1 control function should be executed, which may have any one of a variety of
        * effects, including changing the cursor position, suspending or resuming communications or
        * changing the shift states in effect. There are no parameters to this action.
        */
        virtual void execute(char controlCode) = 0;

        /**
        * This action causes the current private flag, intermediate characters, final character and
        * parameters to be forgotten. This occurs on entry to the escape, csi entry and dcs entry
        * states, so that erroneous sequences like CSI 3 ; 1 CSI 2 J are handled correctly.
        */
        virtual void clear() = 0;

        /**
        * The private marker or intermediate character should be stored for later use in selecting
        * a control function to be executed when a final character arrives. X3.64 doesn’t place any
        * limit on the number of intermediate characters allowed before a final character, although
        * it doesn’t define any control sequences with more than one. Digital defined escape
        * sequences with two intermediate characters, and control sequences and device control
        * strings with one. If more than two intermediate characters arrive, the parser can just
        * flag this so that the dispatch can be turned into a null operation.
        */
        virtual void collect(char value) = 0;

        /**
        * Collects the leading private marker, such as the '?' in `CSI ? Ps h`
        */
        virtual void collectLeader(char leader) = 0;

        /**
        * This action collects the characters of a parameter string for a control sequence or
        * device control sequence and builds a list of parameters. The characters processed by this
        * action are the digits 0-9 (codes 30-39) and the semicolon (code 3B). The semicolon
        * separates parameters. There is no limit to the number of characters in a parameter
        * string, although a maximum of 16 parameters need be stored. If more than 16 parameters
        * arrive, all the extra parameters are silently ignored.
        */
        virtual void param(char value) = 0;

        virtual void paramDigit(char _char) = 0;
        virtual void paramSeparator() = 0;
        virtual void paramSubSeparator() = 0;

        /**
        * The final character of an escape sequence has arrived, so determined the control function
        * to be executed from the intermediate character(s) and final character, and execute it.
        * The intermediate characters are available because collect stored them as they arrived.
        */
        virtual void dispatchESC(char function) = 0;

        /**
        * A final character has arrived, so determine the control function to be executed from
        * private marker, intermediate character(s) and final character, and execute it, passing in
        * the parameter list. The private marker and intermediate characters are available because
        * collect stored them as they arrived.
        */
        virtual void dispatchCSI(char function) = 0;

        /**
        * When the control function OSC (Operating System Command) is recognised,
        * this action initializes an external parser (the “OSC Handler”)
        * to handle the characters from the control string.
        *
        * OSC control strings are not structured in the same way as device control strings,
        * so there is no choice of parsers.
        */
        virtual void startOSC() = 0;

        /**
        * This action passes characters from the control string to the OSC Handler as they arrive.
        * There is therefore no need to buffer characters until the end of the control string is recognised.
        */
        virtual void putOSC(char value) = 0;

        /**
        * This action is called when the OSC string is terminated by ST, CAN, SUB or ESC,
        * to allow the OSC handler to finish neatly.
        */
        virtual void dispatchOSC() = 0;

        /**
        * This action is invoked when a final character arrives in the first part of a device
        * control string. It determines the control function from the private marker, intermediate
        * character(s) and final character, and executes it, passing in the parameter list. It also
        * selects a handler function for the rest of the characters in the control string. This
        * handler function will be called by the put action for every character in the control
        * string as it arrives.
        */
        virtual void hook(char function) = 0;

        /**
        * This action passes characters from the data string part of a device control string to a
        * handler that has previously been selected by the hook action. C0 controls are also passed
        * to the handler.
        */
        virtual void put(char value) = 0;

        /**
        * When a device control string is terminated by ST, CAN, SUB or ESC, this action calls the
        * previously selected handler function with an “end of data” parameter. This allows the
        * handler to finish neatly.
        */
        virtual void unhook() = 0;

        virtual void startAPC() = 0;
        virtual void putAPC(char) = 0;
        virtual void dispatchAPC() = 0;

        virtual void startPM() = 0;
        virtual void putPM(char) = 0;
        virtual void dispatchPM() = 0;
    };

    class NullParserEvents: public IParserEvents
    {
    public:
        void error(std::string_view const&) override {}
        void print(char32_t) override {}
        size_t print(std::string_view, size_t) override { return 0; }
        [[nodiscard]] size_t maxBulkTextSequenceWidth() const noexcept override { return std::numeric_limits<size_t>::max(); }
        void execute(char) override {}
        void clear() override {}
        void collect(char) override {}
        void collectLeader(char) override {}
        void param(char) override {}
        void paramDigit(char /*_char*/) override {}
        void paramSeparator() override {}
        void paramSubSeparator() override {}
        void dispatchESC(char) override {}
        void dispatchCSI(char) override {}
        void startOSC() override {}
        void putOSC(char) override {}
        void dispatchOSC() override {}
        void hook(char) override {}
        void put(char) override {}
        void unhook() override {}
        void startAPC() override {}
        void putAPC(char) override {}
        void dispatchAPC() override {}
        void startPM() override {}
        void putPM(char) override {}
        void dispatchPM() override {}
    };

} // namespace vte::parser
