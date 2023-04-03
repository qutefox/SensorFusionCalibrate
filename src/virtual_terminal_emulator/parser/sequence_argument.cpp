#include "sequence_argument.h"

VTE::Parser::SequenceArgument::SequenceArgument(int value)
    : m_arg{ 0 }
    , m_hasValue{ false }
    , m_isNonFinal{ false }
{
    if (value != -1)
    {
        m_arg = value & 0xffff;
        m_hasValue = true;
    }
}

void VTE::Parser::SequenceArgument::push(uint32_t c)
{
    m_arg = m_arg * 10 + (c - '0');
    /*
    * VT510 tells us to clamp all values to [0, 9999], however, it
    * also allows commands with values up to 2^15-1. We simply use
    * 2^16 as maximum here to be compatible to all commands, but
    * avoid overflows in any calculations.
    */
    if (m_arg > 0xffff) m_arg = 0xffff;
}

void VTE::Parser::SequenceArgument::finish(bool nonfinal)
{
    if (nonfinal) m_isNonFinal = true;
}

void VTE::Parser::SequenceArgument::refinish(bool nonfinal)
{
    m_isNonFinal = nonfinal;
}

bool VTE::Parser::SequenceArgument::hasValue() const
{
    return m_hasValue;
}

bool VTE::Parser::SequenceArgument::isNonfinal() const
{
    return m_isNonFinal;
}

int VTE::Parser::SequenceArgument::getValue(int defaultValue) const
{
    if (m_hasValue) return m_arg;
    return defaultValue;
}

int VTE::Parser::SequenceArgument::getFinalValue(int defaultValue) const
{
    if (m_hasValue && !m_isNonFinal) return m_arg;
    return defaultValue;
}
