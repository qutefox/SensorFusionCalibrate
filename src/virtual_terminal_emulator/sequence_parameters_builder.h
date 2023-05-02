#pragma once

#include "sequence_parameters.h"

namespace vte
{

    /**
     * SequenceParameters builder API.
     *
     * Used to progressively fill a SequenceParameters object.
     *
     * @see SequenceParameters
     */
    class SequenceParameterBuilder
    {
    public:
        using Storage = SequenceParameters::Storage;

        explicit SequenceParameterBuilder(SequenceParameters& p)
            : m_parameters{ p }
            , m_currentParameter{ p.m_values.begin() }
        { }

        void reset()
        {
            m_parameters.clear();
            m_currentParameter = m_parameters.m_values.begin();
        }

        void nextParameter()
        {
            if (m_currentParameter != m_parameters.m_values.end())
            {
                ++m_currentParameter;
                *m_currentParameter = 0;
                m_parameters.m_subParameterTest >>= 1;
            }
        }

        void nextSubParameter()
        {
            if (m_currentParameter != m_parameters.m_values.end())
            {
                ++m_currentParameter;
                *m_currentParameter = 0;
                m_parameters.m_subParameterTest = (m_parameters.m_subParameterTest >> 1) | (1 << 15);
            }
        }

        constexpr void multiplyBy10AndAdd(uint8_t value) noexcept
        {
            *m_currentParameter = static_cast<uint16_t>(*m_currentParameter * 10 + value);
        }

        constexpr void apply(uint16_t value) noexcept
        {
            if (value >= 10)
                multiplyBy10AndAdd(static_cast<uint8_t>(value / 10));
            multiplyBy10AndAdd(static_cast<uint8_t>(value % 10));
        }

        constexpr void set(uint16_t value) noexcept { *m_currentParameter = value; }

        [[nodiscard]] constexpr bool isSubParameter(size_t index) const noexcept
        {
            return (m_parameters.m_subParameterTest & (1 << (count() - 1 - index))) != 0;
        }

        [[nodiscard]] constexpr size_t count() const noexcept
        {
            auto const result =
                std::distance(const_cast<SequenceParameterBuilder*>(this)->m_parameters.m_values.begin(),
                    m_currentParameter)
                + 1;
            if (!(result == 1 && m_parameters.m_values[0] == 0))
                return static_cast<size_t>(result);
            else
                return 0;
        }

        constexpr void fixiate() noexcept
        {
            m_parameters.m_count = count();
            m_parameters.m_subParameterTest >>= 16 - m_parameters.m_count;
        }

    private:
        SequenceParameters& m_parameters;
        Storage::iterator m_currentParameter;
    };

} // namespace vte
