#pragma once

#include <string_view>
#include <cstdint>

#include "sequence.h"

namespace vte
{

    class ISequenceHandler
    {
    public:
        virtual ~ISequenceHandler() = default;

        virtual void executeControlCode(char controlCode) = 0;
        virtual void processSequence(Sequence const& sequence) = 0;
        virtual void writeText(char32_t codepoint) = 0;
        virtual void writeText(std::string_view codepoints, std::size_t cellCount) = 0;
    };

} // namespace vte
