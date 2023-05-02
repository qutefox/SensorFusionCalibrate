#include "sequence.h"

#include <numeric>
#include <sstream>
#include <string>

#include <fmt/format.h>

#include "control_code.h"

namespace vte
{

    std::string Sequence::raw() const
    {
        std::stringstream sstr;

        switch (m_category)
        {
            case FunctionCategory::C0: break;
            case FunctionCategory::ESC: sstr << "\033"; break;
            case FunctionCategory::CSI: sstr << "\033["; break;
            case FunctionCategory::DCS: sstr << "\033P"; break;
            case FunctionCategory::OSC: sstr << "\033]"; break;
        }

        // if (parameterCount() > 1 || (parameterCount() == 1 && m_parameters.at(0) != 0))
        {
            for (size_t i = 0; i < parameterCount(); ++i)
            {
                if (i) sstr << ';';

                sstr << param(i);
                for (size_t k = 1; k < subParameterCount(i); ++k)
                {
                    sstr << ':' << subparam(i, k);
                }
            }
        }

        sstr << intermediateCharacters();

        if (m_finalChar)
            sstr << m_finalChar;

        if (!m_dataString.empty())
            sstr << m_dataString << "\033\\";

        return sstr.str();
    }

    std::string Sequence::text() const
    {
        std::stringstream sstr;

        if (m_category == FunctionCategory::C0)
        {
            sstr << to_short_string(ControlCode::C0(m_finalChar));
            return sstr.str();
        }

        sstr << fmt::format("{}", m_category);

        if (m_leaderSymbol)
            sstr << ' ' << m_leaderSymbol;

        if (parameterCount() > 1 || (parameterCount() == 1 && m_parameters.at(0) != 0))
            sstr << ' ' << m_parameters.str();

        if (!intermediateCharacters().empty())
            sstr << ' ' << intermediateCharacters();

        if (m_finalChar)
            sstr << ' ' << m_finalChar;

        if (!m_dataString.empty())
            sstr << " \"" << vte::escape(m_dataString) << "\" ST";

        return sstr.str();
    }


} // namespace vte
