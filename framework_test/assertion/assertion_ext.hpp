#ifndef ASSERTION_EXT_HPP
#define ASSERTION_EXT_HPP
#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <exception>
#include <type_traits>
#include <iterator>

// add project message/colors for formatted output
#include "../log/message.hpp"
#include "../log/colors.hpp"

namespace assert_ext
{

    struct Result
    {
        bool ok;
        std::string msg;
    };

    inline std::string make_msg(const std::string &desc, bool ok)
    {
        if (desc.empty())
            return ok ? std::string() : std::string("(failed)");
        return ok ? desc : desc + " (failed)";
    }

    inline void print_result(const Result &r, const std::string &name)
    {
        // use tester::Message to format with project colors
        tester::Message m;
        using namespace tester::colors;

        if (r.ok)
        {
            m.SetColor(GREEN) << "[ OK ] " << name;
            if (!r.msg.empty())
                m << " - " << r.msg;
        }
        else
        {
            // failure: red background with white text for visibility
            m.SetColor(std::string(BG_BRIGHT_RED) + std::string(WHITE)) << "[FAIL] " << name;
            if (!r.msg.empty())
                m << " - " << r.msg;
        }
        std::cout << m << "\n";
    }

    // Basic equality/inequality/greater-than
    template <typename A, typename B>
    Result eq(const A &a, const B &b, const std::string &desc = "")
    {
        bool ok;
        if constexpr (std::is_integral_v<A> && std::is_integral_v<B>)
        {
            using C = std::common_type_t<A, B>;
            ok = static_cast<C>(a) == static_cast<C>(b);
        }
        else
        {
            ok = (a == b);
        }
        return {ok, make_msg(desc, ok)};
    }

    template <typename A, typename B>
    Result ne(const A &a, const B &b, const std::string &desc = "")
    {
        bool ok;
        if constexpr (std::is_integral_v<A> && std::is_integral_v<B>)
        {
            using C = std::common_type_t<A, B>;
            ok = !(static_cast<C>(a) == static_cast<C>(b));
        }
        else
        {
            ok = !(a == b);
        }
        return {ok, make_msg(desc, ok)};
    }

    template <typename A, typename B>
    Result gt(const A &a, const B &b, const std::string &desc = "")
    {
        bool ok;
        if constexpr (std::is_integral_v<A> && std::is_integral_v<B>)
        {
            using C = std::common_type_t<A, B>;
            ok = static_cast<C>(a) > static_cast<C>(b);
        }
        else
        {
            ok = (a > b);
        }
        return {ok, make_msg(desc, ok)};
    }

    // approximate equality for floating point values
    template <typename A, typename B>
    Result approx_eq(A a, B b, double tol = 1e-6, const std::string &desc = "")
    {
        double da = static_cast<double>(a);
        double db = static_cast<double>(b);
        bool ok = std::fabs(da - db) <= tol;
        return {ok, make_msg(desc, ok)};
    }

    // ptr_eq overloads: support same-type pointers and pointer vs nullptr comparisons

    // same-type comparison (covers same pointer types and function pointers of same type)
    template <typename T>
    Result ptr_eq(T a, T b, const std::string &desc = "")
    {
        bool ok = (a == b);
        return {ok, make_msg(desc, ok)};
    }

    // pointer (T*) == nullptr
    template <typename T>
    Result ptr_eq(T *a, std::nullptr_t, const std::string &desc = "")
    {
        bool ok = (a == nullptr);
        return {ok, make_msg(desc, ok)};
    }

    // nullptr == pointer (T*)
    template <typename T>
    Result ptr_eq(std::nullptr_t, T *b, const std::string &desc = "")
    {
        bool ok = (b == nullptr);
        return {ok, make_msg(desc, ok)};
    }

    // fallback for void* vs nullptr and void* vs void* comparisons
    inline Result ptr_eq(void *a, void *b, const std::string &desc = "")
    {
        bool ok = (a == b);
        return {ok, make_msg(desc, ok)};
    }
    inline Result ptr_eq(void *a, std::nullptr_t, const std::string &desc = "")
    {
        bool ok = (a == nullptr);
        return {ok, make_msg(desc, ok)};
    }
    inline Result ptr_eq(std::nullptr_t, void *b, const std::string &desc = "")
    {
        bool ok = (b == nullptr);
        return {ok, make_msg(desc, ok)};
    }

    // container equality (element-wise)
    template <typename C1, typename C2>
    Result container_eq(const C1 &c1, const C2 &c2, const std::string &desc = "")
    {
        auto b1 = std::begin(c1), e1 = std::end(c1);
        auto b2 = std::begin(c2), e2 = std::end(c2);
        bool ok = std::distance(b1, e1) == std::distance(b2, e2) &&
                  std::equal(b1, e1, b2);
        return {ok, make_msg(desc, ok)};
    }

    // predicate check: predicate should be callable as pred(value)
    template <typename Pred, typename T>
    Result predicate(Pred p, T val, const std::string &desc = "")
    {
        bool ok = static_cast<bool>(p(val));
        return {ok, make_msg(desc, ok)};
    }

    // exceptions: expect a specific exception type
    template <typename Ex, typename F>
    Result throws(F f, const std::string &desc = "")
    {
        try
        {
            f();
            return {false, make_msg(desc + " (no throw)", false)};
        }
        catch (const Ex &)
        {
            return {true, make_msg(desc, true)};
        }
        catch (...)
        {
            return {false, make_msg(desc + " (wrong exception)", false)};
        }
    }

    template <typename F>
    Result does_not_throw(F f, const std::string &desc = "")
    {
        try
        {
            f();
            return {true, make_msg(desc, true)};
        }
        catch (const std::exception &e)
        {
            return {false, make_msg(desc + std::string(" (threw: ") + e.what() + ")", false)};
        }
        catch (...)
        {
            return {false, make_msg(desc + " (threw unknown)", false)};
        }
    }

} // namespace assert_ext

#endif // ASSERTION_EXT_HPP
