#include "functions.h"

namespace vte
{

    FunctionDefinition const* select(FunctionSelector const& selector) noexcept
    {
        auto static const& funcs = functions();

        auto a = size_t { 0 };
        auto b = funcs.size() - 1;
        while (a <= b)
        {
            auto const i = (a + b) / 2;
            auto const& I = funcs[i];
            auto const rel = compare(selector, I);

            if (rel > 0) a = i + 1;
            else if (rel < 0)
            {
                if (i == 0) return nullptr;
                b = i - 1;
            }
            else return &I;
        }
        return nullptr;
    }

} // namespace vte
