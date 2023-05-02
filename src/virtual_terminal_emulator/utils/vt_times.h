#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

namespace vte
{

namespace detail
{
    template <typename I, typename T>
    struct TimesIterator
    {
        T start;
        I count;
        T step;
        T current;

        constexpr T operator*() noexcept { return current; }
        constexpr T const& operator*() const noexcept { return current; }

        constexpr TimesIterator<I, T>& operator++() noexcept
        {
            current += step;
            --count;
            return *this;
        }
        constexpr TimesIterator<I, T>& operator++(int) noexcept { return ++*this; }

        constexpr TimesIterator<I, T>& operator--() noexcept
        {
            current -= step;
            ++count;
            return *this;
        }
        constexpr TimesIterator<I, T>& operator--(int) noexcept { return ++*this; }

        constexpr bool operator==(TimesIterator<I, T> const& other) const noexcept
        {
            return count == other.count;
        }
        constexpr bool operator!=(TimesIterator<I, T> const& other) const noexcept
        {
            return count != other.count;
        }
    };

    template <typename I, typename T>
    struct Times
    {
        T start;
        I count;
        T step;

        using iterator = TimesIterator<I, T>;

        [[nodiscard]] constexpr std::size_t size() const noexcept { return count; }
        constexpr T operator[](size_t i) const noexcept { return start + i * step; }

        [[nodiscard]] constexpr iterator begin() const noexcept
        {
            return TimesIterator<I, T> { start, count, step, start };
        }

        [[nodiscard]] constexpr iterator end() const noexcept
        {
            return iterator { start, I {}, step, static_cast<T>(start + count * step) };
        }
    };

    template <typename I, typename T>
    Times(T, I, T) -> Times<I, T>;

    template <typename I, typename T>
    constexpr auto begin(Times<I, T> const& times) noexcept
    {
        return times.begin();
    }
    template <typename I, typename T>
    constexpr auto end(Times<I, T> const& times) noexcept
    {
        return times.end();
    }

    template <typename I, typename T>
    constexpr auto begin(Times<I, T>& times) noexcept
    {
        return times.begin();
    }
    template <typename I, typename T>
    constexpr auto end(Times<I, T>& times) noexcept
    {
        return times.end();
    }

} // namespace detail

template <typename I, typename T>
constexpr inline detail::Times<I, T> times(T start, I count, T step = T(1))
{
    return detail::Times<I, T> { start, count, step };
}

template <typename T>
constexpr inline detail::Times<T, T> times(T count)
{
    return detail::Times<T, T> { T(0), count, T(1) };
}

} // namespace vte
