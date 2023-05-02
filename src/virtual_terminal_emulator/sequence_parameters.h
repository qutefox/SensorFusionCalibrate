#pragma once

#include <cstdint>
#include <array>
#include <string>

#include <gsl/span>

namespace vte
{

    class SequenceParameterBuilder;

    /**
    * CSI parameter API.
    *
    * This object holds the numeric parameters as used in a CSI sequence.
    *
    * @note Use SequenceParameterBuilder for filling a SequenceParameters object.
    */
    class SequenceParameters
    {
    public:
        using Storage = std::array<uint16_t, 16>;

        [[nodiscard]] constexpr uint16_t at(std::size_t index) const noexcept { return m_values[index]; }

        [[nodiscard]] constexpr bool isSubParameter(std::size_t index) const noexcept
        {
            return (m_subParameterTest & (1 << index)) != 0;
        }

        /// Returns the number of sub-params for a given non-sub param.
        [[nodiscard]] constexpr std::size_t subParameterCount(std::size_t index) const noexcept
        {
            if (!isSubParameter(index))
            {
                index++;
                auto const start = index;
                while (index < 16 && isSubParameter(index))
                {
                    index++;
                }
                return index - start;
            }
            return 0;
        }

        constexpr void clear() noexcept
        {
            m_values[0] = 0;
            m_subParameterTest = 0;
            m_count = 0;
        }

        [[nodiscard]] constexpr bool empty() const noexcept { return m_count == 0; }
        [[nodiscard]] constexpr std::size_t count() const noexcept { return m_count; }

        [[nodiscard]] constexpr gsl::span<std::uint16_t> range() noexcept
        {
            return gsl::span{ m_values.data(), m_count };
        }

        [[nodiscard]] constexpr gsl::span<std::uint16_t const> range() const noexcept
        {
            return gsl::span{ m_values.data(), m_count };
        }

        [[nodiscard]] std::string str() const
        {
            std::string s;

            auto const e = count();
            for (size_t i = 0; i != e; ++i)
            {
                if (!s.empty())
                    s += isSubParameter(i) ? ':' : ';';

                if (isSubParameter(i) && !m_values[i])
                    continue;

                s += std::to_string(m_values[i]);
            }

            return s;
        }

    private:
        friend class SequenceParameterBuilder;
        Storage m_values{};
        uint16_t m_subParameterTest = 0;
        std::size_t m_count = 0;
    };

} // namespace vte
