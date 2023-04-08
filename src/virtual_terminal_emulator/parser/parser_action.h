#pragma once

#include <limits>
#include <string>
#include <string_view>

namespace vte::parser
{

    enum class ActionClass
    {
        /// Action to be invoked because we enter a new state.
        Enter,
        /// Action to be invoked while not changing state.
        Event,
        /// Action to be invoked because we leave a state.
        Leave,
        /// Action to be invoked upon transitioning from one state to another.
        Transition,
    };

    /// An event may cause one of these actions to occur with or without a change of state.
    enum class Action : uint8_t
    {
        /// Internal state to signal that this action doesn't exist (or hasn't been assigned to).
        Undefined,

        /**
         * The character or control is not processed. No observable difference in the terminal’s
         * state would occur if the character that caused this action was not present in the input
         * stream. (Therefore, this action can only occur within a state.)
         */
        Ignore,

        /**
         * This action only occurs in ground state. The current code should be mapped to a glyph
         * according to the character set mappings and shift states in effect, and that glyph should
         * be displayed. 20 (SP) and 7F (DEL) have special behaviour in later VT series, as
         * described in ground.
         */
        Print,

        /**
         * The C0 or C1 control function should be executed, which may have any one of a variety of
         * effects, including changing the cursor position, suspending or resuming communications or
         * changing the shift states in effect. There are no parameters to this action.
         */
        Execute,

        /**
         * This action causes the current private flag, intermediate characters, final character and
         * parameters to be forgotten. This occurs on entry to the escape, csi entry and dcs entry
         * states, so that erroneous sequences like CSI 3 ; 1 CSI 2 J are handled correctly.
         */
        Clear,

        /**
         * The private marker or intermediate character should be stored for later use in selecting
         * a control function to be executed when a final character arrives. X3.64 doesn’t place any
         * limit on the number of intermediate characters allowed before a final character, although
         * it doesn’t define any control sequences with more than one. Digital defined escape
         * sequences with two intermediate characters, and control sequences and device control
         * strings with one. If more than two intermediate characters arrive, the parser can just
         * flag this so that the dispatch can be turned into a null operation.
         */
        Collect,

        /**
         * Collects the leading private marker, such as the '?' in `CSI ? Ps h`
         */
        CollectLeader,

        /**
         * This action collects the characters of a parameter string for a control sequence or
         * device control sequence and builds a list of parameters. The characters processed by this
         * action are the digits 0-9 (codes 30-39) and the semicolon (code 3B). The semicolon
         * separates parameters. There is no limit to the number of characters in a parameter
         * string, although a maximum of 16 parameters need be stored. If more than 16 parameters
         * arrive, all the extra parameters are silently ignored.
         */
        Param,
        ParamDigit,        // [0-9]
        ParamSeparator,    // ';'
        ParamSubSeparator, // ':'

        // NOLINTBEGIN(readability-identifier-naming)

        /**
         * The final character of an escape sequence has arrived, so determined the control function
         * to be executed from the intermediate character(s) and final character, and execute it.
         * The intermediate characters are available because collect stored them as they arrived.
         */
        ESC_Dispatch,

        /**
         * A final character has arrived, so determine the control function to be executed from
         * private marker, intermediate character(s) and final character, and execute it, passing in
         * the parameter list. The private marker and intermediate characters are available because
         * collect stored them as they arrived.
         */
        CSI_Dispatch,

        /**
         * This action is invoked when a final character arrives in the first part of a device
         * control string. It determines the control function from the private marker, intermediate
         * character(s) and final character, and executes it, passing in the parameter list. It also
         * selects a handler function for the rest of the characters in the control string. This
         * handler function will be called by the put action for every character in the control
         * string as it arrives.
         */
        Hook,

        /**
         * This action passes characters from the data string part of a device control string to a
         * handler that has previously been selected by the hook action. C0 controls are also passed
         * to the handler.
         */
        Put,

        /**
         * When a device control string is terminated by ST, CAN, SUB or ESC, this action calls the
         * previously selected handler function with an “end of data” parameter. This allows the
         * handler to finish neatly.
         */
        Unhook,

        APC_Start,
        APC_Put,
        APC_End,

        PM_Start,
        PM_Put,
        PM_End,

        /**
         * When the control function OSC (Operating System Command) is recognised,
         * this action initializes an external parser (the “OSC Handler”)
         * to handle the characters from the control string.
         *
         * OSC control strings are not structured in the same way as device control strings,
         * so there is no choice of parsers.
         */
        OSC_Start,

        /**
         * This action passes characters from the control string to the OSC Handler as they arrive.
         * There is therefore no need to buffer characters until the end of the control string is recognised.
         */
        OSC_Put,

        /**
         * This action is called when the OSC string is terminated by ST, CAN, SUB or ESC,
         * to allow the OSC handler to finish neatly.
         */
        OSC_End,

        /**
         * This action is called when Ground state is entered. The previous graphic character is then
         * being reset to 0 such that the grapheme cluster segmentation algorithm won't accidentally
         * mix up with older text.
         */
        GroundStart,
    };

    constexpr std::string_view to_string(ActionClass actionClass)
    {
        switch (actionClass)
        {
            case ActionClass::Enter: return "Enter";
            case ActionClass::Event: return "Event";
            case ActionClass::Leave: return "Leave";
            case ActionClass::Transition: return "Transition";
        }
        return "?";
    }

    constexpr std::string_view to_string(Action action)
    {
        switch (action)
        {
            case Action::Undefined: return "Undefined";
            case Action::GroundStart: return "GroundStart";
            case Action::Ignore: return "Ignore";
            case Action::Execute: return "Execute";
            case Action::Print: return "Print";
            case Action::Clear: return "Clear";
            case Action::Collect: return "Collect";
            case Action::CollectLeader: return "CollectLeader";
            case Action::Param: return "Param";
            case Action::ParamDigit: return "ParamDigit";
            case Action::ParamSeparator: return "ParamSeparator";
            case Action::ParamSubSeparator: return "ParamSubSeparator";
            case Action::ESC_Dispatch: return "Escape Dispatch";
            case Action::CSI_Dispatch: return "CSI Dispatch";
            case Action::Hook: return "Hook";
            case Action::Put: return "Put";
            case Action::Unhook: return "Unhook";
            case Action::OSC_Start: return "OSC Start";
            case Action::OSC_Put: return "OSC Put";
            case Action::OSC_End: return "OSC End";
            case Action::APC_Start: return "APC Start";
            case Action::APC_Put: return "APC Put";
            case Action::APC_End: return "APC End";
            case Action::PM_Start: return "PM Start";
            case Action::PM_Put: return "PM Put";
            case Action::PM_End: return "PM End";
        }
        return "?";
    }

} // namespace vte::parser

namespace std
{

    template <>
    struct numeric_limits<vte::parser::Action>
    {
        using Action = vte::parser::Action;
        constexpr static Action min() noexcept { return Action::Ignore; } // skip Undefined
        constexpr static Action max() noexcept { return Action::OSC_End; }
        constexpr static size_t size() noexcept { return 19; }
    };

} // namespace std
