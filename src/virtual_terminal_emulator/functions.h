#pragma once

#include <cstdint>
#include <optional>
#include <array>

#include <fmt/format.h>

#include "utils/vt_sort.h"
#include "utils/vt_escape.h"
#include "vt_type.h"

namespace vte
{

    enum class FunctionCategory : uint8_t
    {
        C0 = 0,
        ESC = 1,
        CSI = 2,
        OSC = 3,
        DCS = 4,
    };

    // Defines a function with all its syntax requirements plus some additional meta information.
    class FunctionDefinition
    {
    public:
        FunctionCategory m_category;  // (3 bits) C0, ESC, CSI, OSC, DCS
        char m_leader;                // (3 bits) 0x3C..0x3F (one of: < = > ?, or 0x00 for none)
        char m_intermediate;          // (4 bits) 0x20..0x2F (intermediates, usually just one, or 0x00 if none)
        char m_finalSymbol;           // (7 bits) 0x30..0x7E (final character)
        uint8_t m_minimumParameters;  // (4 bits) 0..7
        uint16_t m_maximumParameters; // (10 bits) 0..1024 for integer value (OSC function parameter)

        // Conformance level and extension are mutually exclusive.
        // But it is unclear to me whether or not it is guaranteed to always have a constexpr-aware std::variant.
        // So keep it the classic way (for now).
        VTType m_conformanceLevel;
        VTExtension m_extension = VTExtension::None;

        std::string_view m_mnemonic;
        std::string_view m_comment;

        using id_type = uint32_t;

        [[nodiscard]] constexpr id_type id() const noexcept
        {
            unsigned constexpr CategoryShift     = 0;
            unsigned constexpr LeaderShift       = 3;
            unsigned constexpr IntermediateShift = 3 + 3;
            unsigned constexpr FinalShift        = 3 + 3 + 4;
            unsigned constexpr MinParamShift     = 3 + 3 + 4 + 7;
            unsigned constexpr MaxParamShift     = 3 + 3 + 4 + 7 + 4;

            // if (m_category == FunctionCategory::C0)
            //     return static_cast<id_type>(m_category) | m_finalSymbol << 3;

            auto const maskCat = static_cast<id_type>(m_category) << CategoryShift;

            // 0x3C..0x3F; (one of: < = > ?, or 0x00 for none)
            auto const maskLeader = !m_leader ? 0 : (static_cast<id_type>(m_leader) - 0x3C) << LeaderShift;

            // 0x20..0x2F: (intermediates, usually just one, or 0x00 if none)
            auto const maskInterm =
                !m_intermediate ? 0 : (static_cast<id_type>(m_intermediate) - 0x20 + 1) << IntermediateShift;

            // 0x40..0x7E: final character
            auto const maskFinalS = !m_finalSymbol ? 0 : (static_cast<id_type>(m_finalSymbol) - 0x40) << FinalShift;
            auto const maskMinPar = static_cast<id_type>(m_minimumParameters) << MinParamShift;
            auto const maskMaxPar = static_cast<id_type>(m_maximumParameters) << MaxParamShift;

            return maskCat | maskLeader | maskInterm | maskFinalS | maskMinPar | maskMaxPar;
        }

        constexpr operator id_type() const noexcept { return id(); }
    };

    constexpr int compare(FunctionDefinition const& a, FunctionDefinition const& b)
    {
        if (a.m_category != b.m_category)
            return static_cast<int>(a.m_category) - static_cast<int>(b.m_category);

        if (a.m_finalSymbol != b.m_finalSymbol)
            return static_cast<int>(a.m_finalSymbol) - static_cast<int>(b.m_finalSymbol);

        if (a.m_leader != b.m_leader)
            return a.m_leader - b.m_leader;

        if (a.m_intermediate != b.m_intermediate)
            return static_cast<int>(a.m_intermediate) - static_cast<int>(b.m_intermediate);

        if (a.m_minimumParameters != b.m_minimumParameters)
            return static_cast<int>(a.m_minimumParameters) - static_cast<int>(b.m_minimumParameters);

        return static_cast<int>(a.m_maximumParameters) - static_cast<int>(b.m_maximumParameters);
    }

    constexpr bool operator==(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) == 0; }
    constexpr bool operator!=(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) != 0; }
    constexpr bool operator<=(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) <= 0; }
    constexpr bool operator>=(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) >= 0; }
    constexpr bool operator<(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) < 0; }
    constexpr bool operator>(FunctionDefinition const& a, FunctionDefinition const& b) noexcept { return compare(a, b) > 0; }
    
    class FunctionSelector
    {
    public:
        FunctionCategory m_category; /** represents the corresponding function category. */
        char m_leader; /** an optional value between 0x3C .. 0x3F */
        int m_argc; /** number of arguments supplied */
        char m_intermediate; /** an optional intermediate character between (0x20 .. 0x2F) */
        char m_finalSymbol; /** between 0x40 .. 0x7F */
    };

    constexpr int compare(FunctionSelector const& a, FunctionDefinition const& b) noexcept
    {
        if (a.m_category != b.m_category)
            return static_cast<int>(a.m_category) - static_cast<int>(b.m_category);

        if (a.m_finalSymbol != b.m_finalSymbol)
            return a.m_finalSymbol - b.m_finalSymbol;

        if (a.m_leader != b.m_leader)
            return a.m_leader - b.m_leader;

        if (a.m_intermediate != b.m_intermediate)
            return a.m_intermediate - b.m_intermediate;

        if (a.m_category == FunctionCategory::OSC)
            return static_cast<int>(a.m_argc) - static_cast<int>(b.m_maximumParameters);

        if (a.m_argc < b.m_minimumParameters)
            return -1;

        if (a.m_argc > b.m_maximumParameters)
            return +1;

        return 0;
    }

    namespace detail
    {
        constexpr auto C0(char finalCharacter, std::string_view mnemonic,
            std::string_view description, VTType vt = VTType::VT100) noexcept
        {
            return FunctionDefinition { FunctionCategory::C0, 0, 0, finalCharacter, 0, 0, vt,
                VTExtension::None, mnemonic, description };
        }

        constexpr auto OSC(uint16_t code, VTExtension ext, std::string_view mnemonic,
            std::string_view description) noexcept
        {
            return FunctionDefinition { FunctionCategory::OSC, 0, 0, 0, 0, code,
                VTType::VT100, ext, mnemonic, description };
        }

        constexpr auto ESC(std::optional<char> intermediate, char finalCharacter,
            VTType vt, std::string_view mnemonic, std::string_view description) noexcept
        {
            return FunctionDefinition { FunctionCategory::ESC, 0, intermediate.value_or(0),
                finalCharacter, 0, 0, vt, VTExtension::None, mnemonic, description };
        }

        constexpr auto CSI(std::optional<char> leader, uint8_t argc0, uint8_t argc1,
            std::optional<char> intermediate, char finalCharacter, VTType vt,
            std::string_view mnemonic, std::string_view description) noexcept
        {
            // TODO: static_assert on leader/intermediate range-or-null
            return FunctionDefinition { FunctionCategory::CSI, leader.value_or(0),
                intermediate.value_or(0), finalCharacter, argc0, argc1,
                vt, VTExtension::None, mnemonic, description };
        }

        constexpr auto CSI(std::optional<char> leader, uint8_t argc0, uint8_t argc1,
            std::optional<char> intermediate, char finalCharacter, VTExtension ext,
            std::string_view mnemonic, std::string_view description) noexcept
        {
            // TODO: static_assert on leader/intermediate range-or-null
            return FunctionDefinition { FunctionCategory::CSI, leader.value_or(0),
                intermediate.value_or(0), finalCharacter, argc0, argc1,
                VTType::VT100, ext, mnemonic, description };
        }

        constexpr auto DCS(std::optional<char> leader, uint8_t argc0, uint8_t argc1,
            std::optional<char> intermediate, char finalCharacter, VTType vt,
            std::string_view mnemonic, std::string_view description) noexcept
        {
            // TODO: static_assert on leader/intermediate range-or-null
            return FunctionDefinition { FunctionCategory::DCS, leader.value_or(0),
                intermediate.value_or(0), finalCharacter, argc0, argc1,
                vt, VTExtension::None, mnemonic, description };
        }

        constexpr auto DCS(std::optional<char> leader, uint8_t argc0, uint8_t argc1,
            std::optional<char> intermediate, char finalCharacter, VTExtension ext,
            std::string_view mnemonic, std::string_view description) noexcept
        {
            // TODO: static_assert on leader/intermediate range-or-null
            return FunctionDefinition { FunctionCategory::DCS, leader.value_or(0),
                intermediate.value_or(0), finalCharacter, argc0, argc1,
                VTType::VT100, ext, mnemonic, description };
        }
    } // namespace detail

    // C0
    constexpr inline auto EOT = detail::C0('\x04', "EOT", "End of Transmission");
    constexpr inline auto BEL = detail::C0('\x07', "BEL", "Bell");
    constexpr inline auto BS  = detail::C0('\x08', "BS", "Backspace");
    constexpr inline auto TAB = detail::C0('\x09', "TAB", "Tab");
    constexpr inline auto LF  = detail::C0('\x0A', "LF", "Line Feed");
    constexpr inline auto VT  = detail::C0('\x0B', "VT", "Vertical Tab"); // Even though VT means Vertical Tab, it seems that xterm is doing an IND instead.
    constexpr inline auto FF  = detail::C0('\x0C', "FF", "Form Feed");
    constexpr inline auto CR  = detail::C0('\x0D', "CR", "Carriage Return");
    constexpr inline auto LS1 = detail::C0('\x0E', "LS1", "Shift Out; Maps G1 into GL.", VTType::VT220);
    constexpr inline auto LS0 = detail::C0('\x0F', "LS0", "Shift In; Maps G0 into GL (the default).", VTType::VT220);

    // SCS to support (G0, G1, G2, G3)
    // A        UK (British), VT100
    // B        USASCII, VT100
    // 4        Dutch, VT200
    // C
    // S        Finnish, VT200
    // R
    // f        French, VT200
    // Q
    // 9        French Canadian, VT200
    // K        VT200
    // " >      Greek VT500
    // % =      Hebrew VT500
    // Y        Italian, VT200
    // `
    // E
    // 6        Norwegian/Danish, VT200
    // % 6      Portuguese, VT300
    // Z        Spanish, VT200.
    // H
    // 7        Swedish, VT200.
    // =        Swiss, VT200.
    // % 2      Turkish, VT500.

    // ESC functions
    constexpr inline auto SCS_G0_SPECIAL = detail::ESC('(', '0', VTType::VT100, "SCS_G0_SPECIAL", "Set G0 to DEC Special Character and Line Drawing Set");
    constexpr inline auto SCS_G0_USASCII = detail::ESC('(', 'B', VTType::VT100, "SCS_G0_USASCII", "Set G0 to USASCII");
    constexpr inline auto SCS_G1_SPECIAL = detail::ESC(')', '0', VTType::VT100, "SCS_G1_SPECIAL", "Set G1 to DEC Special Character and Line Drawing Set");
    constexpr inline auto SCS_G1_USASCII = detail::ESC(')', 'B', VTType::VT100, "SCS_G1_USASCII", "Set G1 to USASCII");
    constexpr inline auto DECALN  = detail::ESC('#', '8', VTType::VT100, "DECALN", "Screen Alignment Pattern");
    constexpr inline auto DECBI   = detail::ESC(std::nullopt, '6', VTType::VT100, "DECBI", "Back Index");
    constexpr inline auto DECFI   = detail::ESC(std::nullopt, '9', VTType::VT100, "DECFI", "Forward Index");
    constexpr inline auto DECKPAM = detail::ESC(std::nullopt, '=', VTType::VT100, "DECKPAM", "Keypad Application Mode");
    constexpr inline auto DECKPNM = detail::ESC(std::nullopt, '>', VTType::VT100, "DECKPNM", "Keypad Numeric Mode");
    constexpr inline auto DECRS   = detail::ESC(std::nullopt, '8', VTType::VT100, "DECRS", "Restore Cursor");
    constexpr inline auto DECSC   = detail::ESC(std::nullopt, '7', VTType::VT100, "DECSC", "Save Cursor");
    constexpr inline auto HTS     = detail::ESC(std::nullopt, 'H', VTType::VT100, "HTS", "Horizontal Tab Set");
    constexpr inline auto IND     = detail::ESC(std::nullopt, 'D', VTType::VT100, "IND", "Index");
    constexpr inline auto NEL     = detail::ESC(std::nullopt, 'E', VTType::VT100, "NEL", "Next Line");
    constexpr inline auto RI      = detail::ESC(std::nullopt, 'M', VTType::VT100, "RI", "Reverse Index");
    constexpr inline auto RIS     = detail::ESC(std::nullopt, 'c', VTType::VT100, "RIS", "Reset to Initial State (Hard Reset)");
    constexpr inline auto SS2     = detail::ESC(std::nullopt, 'N', VTType::VT220, "SS2", "Single Shift Select (G2 Character Set)");
    constexpr inline auto SS3     = detail::ESC(std::nullopt, 'O', VTType::VT220, "SS3", "Single Shift Select (G3 Character Set)");

    // CSI
    constexpr inline auto ArgsMax = 127; // this is the maximum number that fits into 7 bits.

    // CSI functions
    constexpr inline auto ANSISYSSC   = detail::CSI(std::nullopt, 0, 0, std::nullopt, 'u', VTType::VT100, "ANSISYSSC", "Save Cursor (ANSI.SYS)");
    constexpr inline auto CBT         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'Z', VTType::VT100, "CBT", "Cursor Backward Tabulation");
    constexpr inline auto CHA         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'G', VTType::VT100, "CHA", "Move cursor to column");
    constexpr inline auto CHT         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'I', VTType::VT100, "CHT", "Cursor Horizontal Forward Tabulation");
    constexpr inline auto CNL         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'E', VTType::VT100, "CNL", "Move cursor to next line");
    constexpr inline auto CPL         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'F', VTType::VT100, "CPL", "Move cursor to previous line");
    constexpr inline auto CPR         = detail::CSI(std::nullopt, 1, 1, std::nullopt, 'n', VTType::VT100, "CPR", "Request Cursor position");
    constexpr inline auto CUB         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'D', VTType::VT100, "CUB", "Move cursor backward");
    constexpr inline auto CUD         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'B', VTType::VT100, "CUD", "Move cursor down");
    constexpr inline auto CUF         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'C', VTType::VT100, "CUF", "Move cursor forward");
    constexpr inline auto CUP         = detail::CSI(std::nullopt, 0, 2, std::nullopt, 'H', VTType::VT100, "CUP", "Move cursor to position");
    constexpr inline auto CUU         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'A', VTType::VT100, "CUU", "Move cursor up");
    constexpr inline auto DA1         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'c', VTType::VT100, "DA1", "Send primary device attributes");
    constexpr inline auto DA2         = detail::CSI('>', 0, 1, std::nullopt, 'c', VTType::VT100, "DA2", "Send secondary device attributes");
    constexpr inline auto DA3         = detail::CSI('=', 0, 1, std::nullopt, 'c', VTType::VT100, "DA3", "Send tertiary device attributes");
    constexpr inline auto DCH         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'P', VTType::VT100, "DCH", "Delete characters");
    constexpr inline auto DECCARA     = detail::CSI(std::nullopt, 5, ArgsMax, '$', 'r', VTType::VT420, "DECCARA", "Change Attributes in Rectangular Area");
    constexpr inline auto DECCRA      = detail::CSI(std::nullopt, 0, 8, '$', 'v', VTType::VT420, "DECCRA", "Copy rectangular area");
    constexpr inline auto DECERA      = detail::CSI(std::nullopt, 0, 4, '$', 'z', VTType::VT420, "DECERA", "Erase rectangular area");
    constexpr inline auto DECFRA      = detail::CSI(std::nullopt, 0, 4, '$', 'x', VTType::VT420, "DECFRA", "Fill rectangular area");
    constexpr inline auto DECDC       = detail::CSI(std::nullopt, 0, 1, '\'', '~', VTType::VT420, "DECDC", "Delete column");
    constexpr inline auto DECIC       = detail::CSI(std::nullopt, 0, 1, '\'', '}', VTType::VT420, "DECIC", "Insert column");
    constexpr inline auto DECSCA      = detail::CSI(std::nullopt, 0, 1, '"', 'q', VTType::VT240, "DECSCA", "Select Character Protection Attribute");
    constexpr inline auto DECSED      = detail::CSI('?', 0, 1, std::nullopt, 'J', VTType::VT240, "DECSED", "Selective Erase in Display");
    constexpr inline auto DECSERA     = detail::CSI(std::nullopt, 0, 4, '$', '{', VTType::VT240, "DECSERA", "Selective Erase in Rectangular Area");
    constexpr inline auto DECSEL      = detail::CSI('?', 0, 1, std::nullopt, 'K', VTType::VT240, "DECSEL", "Selective Erase in Line");
    constexpr inline auto XTRESTORE   = detail::CSI('?', 0, ArgsMax, std::nullopt, 'r', VTExtension::XTerm, "XTRESTORE", "Restore DEC private modes.");
    constexpr inline auto XTSAVE      = detail::CSI('?', 0, ArgsMax, std::nullopt, 's', VTExtension::XTerm, "XTSAVE", "Save DEC private modes.");
    constexpr inline auto DECRM       = detail::CSI('?', 1, ArgsMax, std::nullopt, 'l', VTType::VT100, "DECRM", "Reset DEC-mode");
    constexpr inline auto DECRQM      = detail::CSI('?', 1, 1, '$', 'p', VTType::VT100, "DECRQM", "Request DEC-mode");
    constexpr inline auto DECRQM_ANSI = detail::CSI(std::nullopt, 1, 1, '$', 'p', VTType::VT100, "DECRQM_ANSI", "Request ANSI-mode");
    constexpr inline auto DECRQPSR    = detail::CSI(std::nullopt, 1, 1, '$', 'w', VTType::VT320, "DECRQPSR", "Request presentation state report");
    constexpr inline auto DECSCL      = detail::CSI(std::nullopt, 2, 2, '"', 'p', VTType::VT220, "DECSCL", "Set conformance level (DECSCL), VT220 and up.");
    constexpr inline auto DECSCPP     = detail::CSI(std::nullopt, 0, 1, '$', '|', VTType::VT100, "DECSCPP", "Select 80 or 132 Columns per Page");
    constexpr inline auto DECSNLS     = detail::CSI(std::nullopt, 0, 1, '*', '|', VTType::VT420, "DECSNLS", "Select number of lines per screen.");
    constexpr inline auto DECSCUSR    = detail::CSI(std::nullopt, 0, 1, ' ', 'q', VTType::VT520, "DECSCUSR", "Set Cursor Style");
    constexpr inline auto DECSLRM     = detail::CSI(std::nullopt, 2, 2, std::nullopt, 's', VTType::VT420, "DECSLRM", "Set left/right margin");
    constexpr inline auto DECSM       = detail::CSI('?', 1, ArgsMax, std::nullopt, 'h', VTType::VT100, "DECSM", "Set DEC-mode");
    constexpr inline auto DECSTBM     = detail::CSI(std::nullopt, 0, 2, std::nullopt, 'r', VTType::VT100, "DECSTBM", "Set top/bottom margin");
    constexpr inline auto DECSTR      = detail::CSI(std::nullopt, 0, 0, '!', 'p', VTType::VT100, "DECSTR", "Soft terminal reset");
    constexpr inline auto DECXCPR     = detail::CSI(std::nullopt, 0, 0, std::nullopt, '6', VTType::VT100, "DECXCPR", "Request extended cursor position");
    constexpr inline auto DL          = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'M', VTType::VT100, "DL",  "Delete lines");
    constexpr inline auto ECH         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'X', VTType::VT420, "ECH", "Erase characters");
    constexpr inline auto ED          = detail::CSI(std::nullopt, 0, ArgsMax, std::nullopt, 'J', VTType::VT100, "ED",  "Erase in display");
    constexpr inline auto EL          = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'K', VTType::VT100, "EL",  "Erase in line");
    constexpr inline auto HPA         = detail::CSI(std::nullopt, 1, 1, std::nullopt, '`', VTType::VT100, "HPA", "Horizontal position absolute");
    constexpr inline auto HPR         = detail::CSI(std::nullopt, 1, 1, std::nullopt, 'a', VTType::VT100, "HPR", "Horizontal position relative");
    constexpr inline auto HVP         = detail::CSI(std::nullopt, 0, 2, std::nullopt, 'f', VTType::VT100, "HVP", "Horizontal and vertical position");
    constexpr inline auto ICH         = detail::CSI(std::nullopt, 0, 1, std::nullopt, '@', VTType::VT420, "ICH", "Insert character");
    constexpr inline auto IL          = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'L', VTType::VT100, "IL",  "Insert lines");
    constexpr inline auto REP         = detail::CSI(std::nullopt, 1, 1, std::nullopt, 'b', VTType::VT100, "REP", "Repeat the preceding graphic character Ps times");
    constexpr inline auto RM          = detail::CSI(std::nullopt, 1, ArgsMax, std::nullopt, 'l', VTType::VT100, "RM",  "Reset mode");
    constexpr inline auto SCOSC       = detail::CSI(std::nullopt, 0, 0, std::nullopt, 's', VTType::VT100, "SCOSC", "Save Cursor");
    constexpr inline auto SD          = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'T', VTType::VT100, "SD",  "Scroll down (pan up)");
    constexpr inline auto SETMARK     = detail::CSI('>', 0, 0, std::nullopt, 'M', VTExtension::Contour, "XTSETMARK", "Set Vertical Mark (experimental syntax)");
    constexpr inline auto SGR         = detail::CSI(std::nullopt, 0, ArgsMax, std::nullopt, 'm', VTType::VT100, "SGR", "Select graphics rendition");
    constexpr inline auto SM          = detail::CSI(std::nullopt, 1, ArgsMax, std::nullopt, 'h', VTType::VT100, "SM",  "Set mode");
    constexpr inline auto SU          = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'S', VTType::VT100, "SU",  "Scroll up (pan down)");
    constexpr inline auto TBC         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'g', VTType::VT100, "TBC", "Horizontal Tab Clear");
    constexpr inline auto VPA         = detail::CSI(std::nullopt, 0, 1, std::nullopt, 'd', VTType::VT100, "VPA", "Vertical Position Absolute");
    constexpr inline auto WINMANIP    = detail::CSI(std::nullopt, 1, 3, std::nullopt, 't', VTExtension::XTerm, "WINMANIP", "Window Manipulation");
    constexpr inline auto XTSMGRAPHICS= detail::CSI('?', 2, 4, std::nullopt, 'S', VTExtension::XTerm, "XTSMGRAPHICS", "Setting/getting Sixel/ReGIS graphics settings.");
    constexpr inline auto XTPOPCOLORS    = detail::CSI(std::nullopt, 0, ArgsMax, '#', 'Q', VTExtension::XTerm, "XTPOPCOLORS", "Pops the color palette from the palette's saved-stack.");
    constexpr inline auto XTPUSHCOLORS   = detail::CSI(std::nullopt, 0, ArgsMax, '#', 'P', VTExtension::XTerm, "XTPUSHCOLORS", "Pushes the color palette onto the palette's saved-stack.");
    constexpr inline auto XTREPORTCOLORS = detail::CSI(std::nullopt, 0, 0, '#', 'R', VTExtension::XTerm, "XTREPORTCOLORS", "Reports number of color palettes on the stack.");
    constexpr inline auto XTSHIFTESCAPE=detail::CSI('>', 0, 1, std::nullopt, 's', VTExtension::XTerm, "XTSHIFTESCAPE", "Set/reset shift-escape options.");
    constexpr inline auto XTVERSION   = detail::CSI('>', 0, 1, std::nullopt, 'q', VTExtension::XTerm, "XTVERSION", "Query terminal name and version");
    constexpr inline auto XTCAPTURE   = detail::CSI('>', 0, 2, std::nullopt, 't', VTExtension::Contour, "XTCAPTURE", "Report screen buffer capture.");

    constexpr inline auto DECSSDT     = detail::CSI(std::nullopt, 0, 1, '$', '~', VTType::VT320, "DECSSDT", "Select Status Display (Line) Type");
    constexpr inline auto DECSASD     = detail::CSI(std::nullopt, 0, 1, '$', '}', VTType::VT420, "DECSASD", "Select Active Status Display");
    constexpr inline auto DECPS       = detail::CSI(std::nullopt, 3, 18, ',', '~', VTType::VT520, "DECPS", "Controls the sound frequency or notes");

    // DCS functions
    constexpr inline auto STP         = detail::DCS(std::nullopt, 0, 0, '$', 'p', VTExtension::Contour, "XTSETPROFILE", "Set Terminal Profile");
    constexpr inline auto DECRQSS     = detail::DCS(std::nullopt, 0, 0, '$', 'q', VTType::VT420, "DECRQSS", "Request Status String");
    constexpr inline auto DECSIXEL    = detail::DCS(std::nullopt, 0, 3, std::nullopt, 'q', VTType::VT330, "DECSIXEL", "Sixel Graphics Image");
    constexpr inline auto XTGETTCAP   = detail::DCS(std::nullopt, 0, 0, '+', 'q', VTExtension::XTerm, "XTGETTCAP", "Request Termcap/Terminfo String");

    // OSC
    constexpr inline auto SETTITLE      = detail::OSC(0, VTExtension::XTerm, "SETINICON", "Change Window & Icon Title");
    constexpr inline auto SETICON       = detail::OSC(1, VTExtension::XTerm, "SETWINICON", "Change Icon Title");
    constexpr inline auto SETWINTITLE   = detail::OSC(2, VTExtension::XTerm, "SETWINTITLE", "Change Window Title");
    constexpr inline auto SETXPROP      = detail::OSC(3, VTExtension::XTerm, "SETXPROP", "Set X11 property");
    constexpr inline auto SETCOLPAL     = detail::OSC(4, VTExtension::XTerm, "SETCOLPAL", "Set/Query color palette");
    // TODO: Ps = 4 ; c ; spec -> Change Color Number c to the color specified by spec.
    // TODO: Ps = 5 ; c ; spec -> Change Special Color Number c to the color specified by spec.
    // TODO: Ps = 6 ; c ; f -> Enable/disable Special Color Number c.
    // TODO: Ps = 7 (set current working directory)
    constexpr inline auto SETCWD        = detail::OSC(7, VTExtension::XTerm, "SETCWD", "Set current working directory");
    constexpr inline auto HYPERLINK     = detail::OSC(8, VTExtension::Unknown, "HYPERLINK", "Hyperlinked Text");
    constexpr inline auto COLORFG       = detail::OSC(10, VTExtension::XTerm, "COLORFG", "Change or request text foreground color.");
    constexpr inline auto COLORBG       = detail::OSC(11, VTExtension::XTerm, "COLORBG", "Change or request text background color.");
    constexpr inline auto COLORCURSOR   = detail::OSC(12, VTExtension::XTerm, "COLORCURSOR", "Change text cursor color to Pt.");
    constexpr inline auto COLORMOUSEFG  = detail::OSC(13, VTExtension::XTerm, "COLORMOUSEFG", "Change mouse foreground color.");
    constexpr inline auto COLORMOUSEBG  = detail::OSC(14, VTExtension::XTerm, "COLORMOUSEBG", "Change mouse background color.");
    constexpr inline auto SETFONT       = detail::OSC(50, VTExtension::XTerm, "SETFONT", "Get or set font.");
    constexpr inline auto SETFONTALL    = detail::OSC(60, VTExtension::Contour, "SETFONTALL", "Get or set all font faces, styles, size.");
    // printf "\033]52;c;$(printf "%s" "blabla" | base64)\a"
    constexpr inline auto CLIPBOARD     = detail::OSC(52, VTExtension::XTerm, "CLIPBOARD", "Clipboard management.");
    constexpr inline auto RCOLPAL       = detail::OSC(104, VTExtension::XTerm, "RCOLPAL", "Reset color full palette or entry");
    constexpr inline auto COLORSPECIAL  = detail::OSC(106, VTExtension::XTerm, "COLORSPECIAL", "Enable/disable Special Color Number c.");
    constexpr inline auto RCOLORFG      = detail::OSC(110, VTExtension::XTerm, "RCOLORFG", "Reset VT100 text foreground color.");
    constexpr inline auto RCOLORBG      = detail::OSC(111, VTExtension::XTerm, "RCOLORBG", "Reset VT100 text background color.");
    constexpr inline auto RCOLORCURSOR  = detail::OSC(112, VTExtension::XTerm, "RCOLORCURSOR", "Reset text cursor color.");
    constexpr inline auto RCOLORMOUSEFG = detail::OSC(113, VTExtension::XTerm,"RCOLORMOUSEFG", "Reset mouse foreground color.");
    constexpr inline auto RCOLORMOUSEBG = detail::OSC(114, VTExtension::XTerm,"RCOLORMOUSEBG", "Reset mouse background color.");
    constexpr inline auto RCOLORHIGHLIGHTFG = detail::OSC(119, VTExtension::XTerm,"RCOLORHIGHLIGHTFG", "Reset highlight foreground color.");
    constexpr inline auto RCOLORHIGHLIGHTBG = detail::OSC(117, VTExtension::XTerm,"RCOLORHIGHLIGHTBG", "Reset highlight background color.");
    constexpr inline auto NOTIFY        = detail::OSC(777, VTExtension::XTerm,"NOTIFY", "Send Notification.");
    constexpr inline auto DUMPSTATE     = detail::OSC(888, VTExtension::Contour, "DUMPSTATE", "Dumps internal state to debug stream.");

    constexpr inline auto CaptureBufferCode = 314;

    inline auto const& functions() noexcept
    {
        static auto const funcs = []() constexpr {
            auto f = std::array {
                // C0
                EOT,
                BEL,
                BS,
                TAB,
                LF,
                VT,
                FF,
                CR,
                LS0,
                LS1,

                // ESC
                DECALN,
                DECBI,
                DECFI,
                DECKPAM,
                DECKPNM,
                DECRS,
                DECSC,
                HTS,
                IND,
                NEL,
                RI,
                RIS,
                SCS_G0_SPECIAL,
                SCS_G0_USASCII,
                SCS_G1_SPECIAL,
                SCS_G1_USASCII,
                SS2,
                SS3,

                // CSI
                ANSISYSSC,
                XTCAPTURE,
                CBT,
                CHA,
                CHT,
                CNL,
                CPL,
                CPR,
                CUB,
                CUD,
                CUF,
                CUP,
                CUU,
                DA1,
                DA2,
                DA3,
                DCH,
                DECCARA,
                DECCRA,
                DECDC,
                DECERA,
                DECFRA,
                DECIC,
                DECSCA,
                DECSED,
                DECSERA,
                DECSEL,
                XTRESTORE,
                XTSAVE,
                DECPS,
                DECRM,
                DECRQM,
                DECRQM_ANSI,
                DECRQPSR,
                DECSASD,
                DECSCL,
                DECSCPP,
                DECSCUSR,
                DECSLRM,
                DECSM,
                DECSNLS,
                DECSSDT,
                DECSTBM,
                DECSTR,
                DECXCPR,
                DL,
                ECH,
                ED,
                EL,
                HPA,
                HPR,
                HVP,
                ICH,
                IL,
                REP,
                RM,
                SCOSC,
                SD,
                SETMARK,
                SGR,
                SM,
                SU,
                TBC,
                VPA,
                WINMANIP,
                XTPOPCOLORS,
                XTPUSHCOLORS,
                XTREPORTCOLORS,
                XTSHIFTESCAPE,
                XTSMGRAPHICS,
                XTVERSION,

                // DCS
                STP,
                DECRQSS,
                DECSIXEL,
                XTGETTCAP,

                // OSC
                SETICON,
                SETTITLE,
                SETWINTITLE,
                SETXPROP,
                SETCOLPAL,
                SETCWD,
                HYPERLINK,
                COLORFG,
                COLORBG,
                COLORCURSOR,
                COLORMOUSEFG,
                COLORMOUSEBG,
                SETFONT,
                SETFONTALL,
                CLIPBOARD,
                RCOLPAL,
                COLORSPECIAL,
                RCOLORFG,
                RCOLORBG,
                RCOLORCURSOR,
                RCOLORMOUSEFG,
                RCOLORMOUSEBG,
                RCOLORHIGHLIGHTFG,
                RCOLORHIGHLIGHTBG,
                NOTIFY,
                DUMPSTATE,
            };
            vte::sort(f, [](FunctionDefinition const& a, FunctionDefinition const& b) constexpr
            {
                return compare(a, b);
            });
            return f;
        }();

        return funcs;
    }

    /**
    * /brief Selects a FunctionDefinition based on a FunctionSelector.
    * @return the matching FunctionDefinition or nullptr if none matched.
    */
    FunctionDefinition const* select(FunctionSelector const& selector) noexcept;

    /**
    * /brief Selects a FunctionDefinition based on given input Escape sequence fields.
    *
    * @param[in] intermediate: an optional intermediate character between (0x20 .. 0x2F)
    * @param[in] finalCharacter: between 0x40 .. 0x7F
    *
    * @notice multi-character intermediates are intentionally not supported.
    *
    * @return the matching FunctionDefinition or nullptr if none matched.
    */
    inline FunctionDefinition const* selectEscape(char intermediate, char finalCharacter)
    {
        return select({ FunctionCategory::ESC, 0, 0, intermediate, finalCharacter });
    }

    /**
    * /brief Selects a FunctionDefinition based on given input control sequence fields.
    *
    * @param[in] leader: an optional value between 0x3C .. 0x3F
    * @param[in] argc: number of arguments supplied
    * @param[in] intermediate: an optional intermediate character between (0x20 .. 0x2F)
    * @param[in] finalCharacter: between 0x40 .. 0x7F
    *
    * @notice multi-character intermediates are intentionally not supported.
    *
    * @return the matching FunctionDefinition or nullptr if none matched.
    */
    inline FunctionDefinition const* selectControl(char leader, int argc, char intermediate, char finalCharacter)
    {
        return select({ FunctionCategory::CSI, leader, argc, intermediate, finalCharacter });
    }

    /**
    * /brief Selects a FunctionDefinition based on given input control sequence fields.
    *
    * @param[in] id: leading numeric identifier (such as 8 for hyperlink)
    *
    * @notice multi-character intermediates are intentionally not supported.
    *
    * @return the matching FunctionDefinition or nullptr if none matched.
    */
    inline FunctionDefinition const* selectOSCommand(int id)
    {
        return select({ FunctionCategory::OSC, 0, id, 0, 0 });
    }

} // namespace vte

namespace std
{
template <>
struct hash<vte::FunctionDefinition>
{
    /// This is actually perfect hashing.
    constexpr uint32_t operator()(vte::FunctionDefinition const& fun) const noexcept { return fun.id(); }
};
} // namespace std

namespace fmt // {{{
{
template <>
struct formatter<vte::FunctionCategory>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const vte::FunctionCategory value, FormatContext& ctx)
    {
        using vte::FunctionCategory;
        switch (value)
        {
            case FunctionCategory::C0: return fmt::format_to(ctx.out(), "C0");
            case FunctionCategory::ESC: return fmt::format_to(ctx.out(), "ESC");
            case FunctionCategory::CSI: return fmt::format_to(ctx.out(), "CSI");
            case FunctionCategory::OSC: return fmt::format_to(ctx.out(), "OSC");
            case FunctionCategory::DCS: return fmt::format_to(ctx.out(), "DCS");
        }
        return fmt::format_to(ctx.out(), "({})", static_cast<unsigned>(value));
    }
};

template <>
struct formatter<vte::FunctionDefinition>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const vte::FunctionDefinition f, FormatContext& ctx)
    {
        switch (f.m_category)
        {
            case vte::FunctionCategory::C0:
                return fmt::format_to(ctx.out(), "{}", vte::escape(static_cast<uint8_t>(f.m_finalSymbol)));
            case vte::FunctionCategory::ESC:
                return fmt::format_to(ctx.out(),
                    "{} {} {}",
                    f.m_category,
                    f.m_intermediate ? f.m_intermediate : ' ',
                    f.m_finalSymbol ? f.m_finalSymbol : ' ');
            case vte::FunctionCategory::OSC:
                return fmt::format_to(ctx.out(), "{} {}", f.m_category, f.m_maximumParameters);
            case vte::FunctionCategory::DCS:
            case vte::FunctionCategory::CSI:
                if (f.m_minimumParameters == f.m_maximumParameters)
                    return fmt::format_to(ctx.out(),
                        "{} {} {}    {} {}",
                        f.m_category,
                        f.m_leader ? f.m_leader : ' ',
                        f.m_minimumParameters,
                        f.m_intermediate ? f.m_intermediate : ' ',
                        f.m_finalSymbol);
                else if (f.m_maximumParameters == vte::ArgsMax)
                    return fmt::format_to(ctx.out(),
                        "{} {} {}..  {} {}",
                        f.m_category,
                        f.m_leader ? f.m_leader : ' ',
                        f.m_minimumParameters,
                        f.m_intermediate ? f.m_intermediate : ' ',
                        f.m_finalSymbol);
                else
                    return fmt::format_to(ctx.out(),
                        "{} {} {}..{} {} {}",
                        f.m_category,
                        f.m_leader ? f.m_leader : ' ',
                        f.m_minimumParameters,
                        f.m_maximumParameters,
                        f.m_intermediate ? f.m_intermediate : ' ',
                        f.m_finalSymbol);
        }
        return fmt::format_to(ctx.out(), "?");
    }
};

template <>
struct formatter<vte::FunctionSelector>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const vte::FunctionSelector f, FormatContext& ctx)
    {
        switch (f.m_category)
        {
            case vte::FunctionCategory::OSC:
                return fmt::format_to(ctx.out(), "{} {}", f.m_category, f.m_argc);
            default:
                return fmt::format_to(ctx.out(),
                    "{} {} {} {} {}",
                    f.m_category,
                    f.m_leader ? f.m_leader : ' ',
                    f.m_argc,
                    f.m_intermediate ? f.m_intermediate : ' ',
                    f.m_finalSymbol ? f.m_finalSymbol : ' ');
}
}
};
} // namespace fmt