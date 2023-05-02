#pragma once

#include <array>
#include "vt_type.h"

namespace vte
{

    auto const append = [](std::string& s, std::string_view const& v)
    {
        if (!s.empty()) s += ",";
        s += v;
    };

    auto constexpr strMapping = std::array<std::pair<DeviceAttributes, std::string_view>, 11>
    {
        std::pair { DeviceAttributes::AnsiColor, "AnsiColor" },
        std::pair { DeviceAttributes::AnsiTextLocator, "AnsiTextLocator" },
        std::pair { DeviceAttributes::Columns132, "Columns132" },
        std::pair { DeviceAttributes::NationalReplacementCharacterSets, "NationalReplacementCharacterSets" },
        std::pair { DeviceAttributes::Printer, "Printer" },
        std::pair { DeviceAttributes::RectangularEditing, "RectangularEditing" },
        std::pair { DeviceAttributes::SelectiveErase, "SelectiveErase" },
        std::pair { DeviceAttributes::SixelGraphics, "SixelGraphics" },
        std::pair { DeviceAttributes::TechnicalCharacters, "TechnicalCharacters" },
        std::pair { DeviceAttributes::UserDefinedKeys, "UserDefinedKeys" },
        std::pair { DeviceAttributes::Windowing, "Windowing" },
    };

    auto constexpr paramMapping = std::array<std::pair<DeviceAttributes, std::string_view>, 12>
    {
        std::pair { DeviceAttributes::AnsiColor, "22" },
        std::pair { DeviceAttributes::AnsiTextLocator, "29" },
        std::pair { DeviceAttributes::CaptureScreenBuffer, "314" },
        std::pair { DeviceAttributes::Columns132, "1" },
        std::pair { DeviceAttributes::NationalReplacementCharacterSets, "9" },
        std::pair { DeviceAttributes::Printer, "2" },
        std::pair { DeviceAttributes::RectangularEditing, "28" },
        std::pair { DeviceAttributes::SelectiveErase, "6" },
        std::pair { DeviceAttributes::SixelGraphics, "4" },
        std::pair { DeviceAttributes::TechnicalCharacters, "15" },
        std::pair { DeviceAttributes::UserDefinedKeys, "8" },
        std::pair { DeviceAttributes::Windowing, "18" },
    };

    std::string to_string(DeviceAttributes v)
    {
        std::string s;
        for (auto const& mapping : strMapping)
        {
            if (v & mapping.first)
            {
                append(s, mapping.second);
            }       
        }
        return s;
    }

    std::string to_params(DeviceAttributes v)
    {
        std::string s;
        for (auto const& mapping: paramMapping)
        {
            if (v & mapping.first)
            {
                append(s, mapping.second);
            }
        }
        return s;
    }

} // namespace vte
