#pragma once

#include "functions.h"
#include "utils/vt_boxed.h"
#include "sequence_parameters_builder.h"

namespace vte
{

    /**
     * Helps constructing VT functions as they're being parsed by the VT parser.
     */
    class Sequence
    {
    public:
        static constexpr std::size_t MAX_OSC_LEN = 512;

        using Parameter = uint16_t;
        using Intermediaries = std::string;
        using DataString = std::string;
        using Parameters = SequenceParameters;

    private:
        FunctionCategory m_category = {};
        char m_leaderSymbol = 0;
        Parameters m_parameters;
        Intermediaries m_intermediateCharacters;
        char m_finalChar = 0;
        DataString m_dataString;

    public:
        // parameter accessors

        [[nodiscard]] Parameters& parameters() noexcept { return m_parameters; }
        [[nodiscard]] Parameters const& parameters() const noexcept { return m_parameters; }

        [[nodiscard]] size_t parameterCount() const noexcept { return m_parameters.count(); }
        [[nodiscard]] size_t subParameterCount(size_t i) const noexcept
        {
            return m_parameters.subParameterCount(i);
        }

        // mutators
        //
        void clear()
        {
            clearExceptParameters();
            m_parameters.clear();
        }

        void clearExceptParameters()
        {
            m_category = FunctionCategory::C0;
            m_leaderSymbol = 0;
            m_intermediateCharacters.clear();
            m_finalChar = 0;
            m_dataString.clear();
        }

        void setCategory(FunctionCategory cat) noexcept { m_category = cat; }
        void setLeader(char ch) noexcept { m_leaderSymbol = ch; }
        [[nodiscard]] Intermediaries& intermediateCharacters() noexcept { return m_intermediateCharacters; }
        void setFinalChar(char ch) noexcept { m_finalChar = ch; }

        [[nodiscard]] DataString const& dataString() const noexcept { return m_dataString; }
        [[nodiscard]] DataString& dataString() noexcept { return m_dataString; }

        /// @returns this VT-sequence into a human readable string form.
        [[nodiscard]] std::string text() const;

        /// @returns the raw VT-sequence string.
        [[nodiscard]] std::string raw() const;

        [[nodiscard]] FunctionDefinition const* functionDefinition() const noexcept { return select(selector()); }

        /// Converts a FunctionSpinto a FunctionSelector, applicable for finding the corresponding
        /// FunctionDefinition.
        [[nodiscard]] FunctionSelector selector() const noexcept
        {
            switch (m_category)
            {
                case FunctionCategory::OSC:
                    return FunctionSelector { m_category, 0, static_cast<int>(parameterCount() ? param(0) : 0), 0, 0 };
                default:
                {
                    // Only support CSI sequences with 0 or 1 intermediate characters.
                    char const intermediate = m_intermediateCharacters.size() == 1
                        ? static_cast<char>(m_intermediateCharacters[0])
                        : char {};

                    return FunctionSelector {
                        m_category, m_leaderSymbol, static_cast<int>(parameterCount()), intermediate, m_finalChar
                    };
                }
            }
        }

        // accessors
        //
        [[nodiscard]] FunctionCategory category() const noexcept { return m_category; }
        [[nodiscard]] Intermediaries const& intermediateCharacters() const noexcept
        {
            return m_intermediateCharacters;
        }
        [[nodiscard]] char leaderSymbol() const noexcept { return m_leaderSymbol; }
        [[nodiscard]] char finalChar() const noexcept { return m_finalChar; }

        template <typename T = unsigned>
        [[nodiscard]] std::optional<T> param_opt(size_t parameterIndex) const noexcept
        {
            if (parameterIndex < m_parameters.count())
            {
                if constexpr (vte::is_boxed<T>)
                    return { T::cast_from(m_parameters.at(parameterIndex)) };
                else
                    return { static_cast<T>(m_parameters.at(parameterIndex)) };
            }
            else
                return std::nullopt;
        }

        template <typename T = unsigned>
        [[nodiscard]] T param_or(size_t parameterIndex, T defaultValue) const noexcept
        {
            return param_opt<T>(parameterIndex).value_or(defaultValue);
        }

        template <typename T = unsigned>
        [[nodiscard]] T param(size_t parameterIndex) const noexcept
        {
            assert(parameterIndex < m_parameters.count());
            if constexpr (vte::is_boxed<T>)
                return T::cast_from(m_parameters.at(parameterIndex));
            else
                return static_cast<T>(m_parameters.at(parameterIndex));
        }

        template <typename T = unsigned>
        [[nodiscard]] T subparam(size_t parameterIndex, size_t subIndex) const noexcept
        {
            return param<T>(parameterIndex + subIndex);
        }

        [[nodiscard]] bool isSubParameter(size_t parameterIndex) const noexcept
        {
            return m_parameters.isSubParameter(parameterIndex);
        }

        template <typename T = unsigned>
        [[nodiscard]] bool containsParameter(T value) const noexcept
        {
            for (size_t i = 0; i < parameterCount(); ++i)
                if constexpr (vte::is_boxed<T>)
                {
                    if (T::cast_from(m_parameters.at(i)) == value)
                        return true;
                }
                else
                {
                    if (T(m_parameters.at(i)) == value)
                        return true;
                }
            return false;
        }
    };

} // namespace vte
