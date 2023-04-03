#pragma once

#include <cstdint>

namespace VTE
{
namespace Parser
{

class SequenceArgument
{
    int m_arg = 0;
    bool m_hasValue = false;
    bool m_isNonFinal = false;

public:
    SequenceArgument(int value = -1);

    void push(uint32_t c);
    void finish(bool nonfinal = false);
    void refinish(bool nonfinal = false);

    bool hasValue() const;
    bool isNonfinal() const;

    int getValue(int defaultValue = -1) const;
    int getFinalValue(int defaultValue = -1) const;
};

} // namespace Parser
} // namespace VTE
