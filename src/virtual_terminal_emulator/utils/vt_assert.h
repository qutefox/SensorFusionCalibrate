#pragma once

#include <fmt/format.h>

#include <exception>
#include <functional>
#include <string_view>

namespace vte
{

// XXX Too bad gsl_assert.h is imported SOMEWHERE THAT IS NOT ME and hence
//     I cannot *just* define Expects() nor Ensures().
// XXX Also (rant!), Expects() and Ensures() is hard-defined (WTF!!!) in GSL.
//     Microsoft, please fix this!
//
// So instead we use:
//   Require()
//   Guarantee()

/// Function signature for custom assertion failure handlers.
using fail_handler_t = std::function<void(std::string_view, std::string_view, std::string_view, int)>;

#if defined(__GNUC__)
// GCC 4.8+, Clang, Intel and other compilers compatible with GCC (-std=c++0x or above)
[[noreturn]] inline __attribute__((always_inline)) void unreachable()
{
    __builtin_unreachable();
}

#elif defined(_MSC_VER) // MSVC

[[noreturn]] __forceinline void unreachable()
{
    __assume(false);
}

#else

[[noreturn]] inline void unreachable()
{
}

#endif

namespace detail
{
    inline fail_handler_t& fail_handler() noexcept
    {
        static fail_handler_t storage {};
        return storage;
    }

    [[noreturn]] inline void fail(std::string_view text, std::string_view message,
        std::string_view file, int line) noexcept
    {
        try
        {
            if (fail_handler())
                fail_handler()(text, message, file, line);
            else
                fmt::print("[{}:{}] {} {}\n", file, line, message, text);
        }
        catch (...)
        {
        }
        std::abort();
    }
} // namespace detail

/// Sets a custom fail handler to be invoked when Expects() or Ensures() fails.
///
/// This handler is supposed to report and terminate but may very well
/// just ignore either or both.
inline void set_fail_handler(fail_handler_t handler)
{
    detail::fail_handler() = std::move(handler);
}

/// This method prints an error message and then terminates the program.
[[noreturn]] inline void todo(std::string_view message = {})
{
    fmt::print("TODO: We have reached some code that is missing an implementation.\n");
    if (!message.empty())
        fmt::print("{}\n", message);
    std::abort();
}

[[noreturn]] inline void fatal(std::string_view message)
{
    fmt::print("Fatal error. {}", message);
    std::abort();
}

#define Require(cond)                                                                \
    do                                                                               \
    {                                                                                \
        if (!(cond))                                                                 \
        {                                                                            \
            vte::detail::fail(#cond, "Precondition failed.", __FILE__, __LINE__); \
        }                                                                            \
    } while (0)

#define Guarantee(cond)                                                               \
    do                                                                                \
    {                                                                                 \
        if (!(cond))                                                                  \
        {                                                                             \
            vte::detail::fail(#cond, "Postcondition failed.", __FILE__, __LINE__); \
        }                                                                             \
    } while (0)

} // namespace vte