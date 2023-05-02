#pragma once

#include <cstdint>
#include <string>

#include <fmt/format.h>

#include "utils/vt_assert.h"

namespace vte
{

    /**
     * Virtual Terminal Types.
     *
     * The integer representation must match the terminalID response encoding.
     *
     * The integer representational values match the one for DA2's first response parameter.
     */
    enum class VTType
    {
        VT100 = 0,
        VT220 = 1,
        VT240 = 2,
        VT330 = 18,
        VT340 = 19,
        VT320 = 24,
        VT420 = 41,
        VT510 = 61,
        VT520 = 64,
        VT525 = 65,
    };

    enum class VTExtension
    {
        None,
        Unknown,
        XTerm,
        Contour,
    };

    /**
     * Defines a set of feature flags a virtual terminal can support.
     *
     * Used in response to SendDeviceAttributes.
     */
    enum class DeviceAttributes : uint16_t
    {
        Columns132 = (1 << 0),
        Printer = (1 << 1),
        SelectiveErase = (1 << 2),
        UserDefinedKeys = (1 << 3),
        NationalReplacementCharacterSets = (1 << 4),
        TechnicalCharacters = (1 << 5),
        AnsiColor = (1 << 6),
        AnsiTextLocator = (1 << 7),
        SixelGraphics = (1 << 8),
        RectangularEditing = (1 << 9),
        Windowing = (1 << 10),
        CaptureScreenBuffer = (1 << 11),
    };

    constexpr DeviceAttributes operator|(DeviceAttributes a, DeviceAttributes b)
    {
        return static_cast<DeviceAttributes>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }

    constexpr bool operator&(DeviceAttributes a, DeviceAttributes b)
    {
        return (static_cast<unsigned>(a) & static_cast<unsigned>(b)) != 0;
    }

    //! Generates human readable string of comma seperated list of attribute names.
    std::string to_string(DeviceAttributes v);

    //! Generates a parameter list that can be used to generate the CSI response.
    std::string to_params(DeviceAttributes v);

} // namespace vte

namespace fmt
{
template <>
struct formatter<vte::VTType>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const vte::VTType id, FormatContext& ctx)
    {
        switch (id)
        {
            case vte::VTType::VT100: return fmt::format_to(ctx.out(), "VT100");
            case vte::VTType::VT220: return fmt::format_to(ctx.out(), "VT220");
            case vte::VTType::VT240: return fmt::format_to(ctx.out(), "VT240");
            case vte::VTType::VT320: return fmt::format_to(ctx.out(), "VT320");
            case vte::VTType::VT330: return fmt::format_to(ctx.out(), "VT330");
            case vte::VTType::VT340: return fmt::format_to(ctx.out(), "VT340");
            case vte::VTType::VT420: return fmt::format_to(ctx.out(), "VT420");
            case vte::VTType::VT510: return fmt::format_to(ctx.out(), "VT510");
            case vte::VTType::VT520: return fmt::format_to(ctx.out(), "VT520");
            case vte::VTType::VT525: return fmt::format_to(ctx.out(), "VT525");
        }
        vte::unreachable();
    }
};
template <>
struct formatter<vte::VTExtension>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const vte::VTExtension id, FormatContext& ctx)
    {
        switch (id)
        {
            case vte::VTExtension::None: return fmt::format_to(ctx.out(), "none");
            case vte::VTExtension::Unknown: return fmt::format_to(ctx.out(), "unknown");
            case vte::VTExtension::XTerm: return fmt::format_to(ctx.out(), "XTerm");
            case vte::VTExtension::Contour: return fmt::format_to(ctx.out(), "Contour");
        }
        vte::unreachable();
    }
};
} // namespace fmt
