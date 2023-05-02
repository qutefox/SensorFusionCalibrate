#pragma once

#include "vt_times.h"

#include <utility>

namespace vte
{

namespace detail
{
    template <typename Container, typename Comp, typename size_type>
    constexpr size_type partition(Container& container, Comp compare, size_type low, size_type high)
    {
        auto i = low - 1;
        auto& pivot = container[high];

        for (auto const j : vte::times(low, static_cast<decltype(low)>(high - low)))
        {
            if (compare(container[j], pivot) <= 0)
            {
                i++;
                std::swap(container[i], container[j]);
            }
        }

        i++;
        std::swap(container[i], container[high]);
        return i;
    }
} // namespace detail

template <typename Container, typename Comp>
constexpr void sort(Container& container, Comp compare, size_t low, size_t high)
{
    if (low < high)
    {
        auto const pi = detail::partition(container, compare, low, high);
        if (pi > 0)
        {
            sort(container, compare, low, pi - 1);
        }
        sort(container, compare, pi + 1, high);
    }
}

template <typename Container, typename Comp>
constexpr void sort(Container& container, Comp compare)
{
    if (auto const count = std::size(container); count > 1)
    {
        sort(container, compare, 0, count - 1);
    }
}

template <typename Container>
constexpr void sort(Container& container)
{
    if (auto const count = std::size(container); count > 1)
        sort(
            container,
            [](auto const& a, auto const& b) { return a < b   ? -1
                : a > b ? +1
                    : 0; },
            0,
            count - 1);
}

} // namespace vte
