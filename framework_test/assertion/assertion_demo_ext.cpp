#include "assertion_ext.hpp"
#include "../log/message.hpp"
#include "../log/colors.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

int main()
{
    using namespace assert_ext;
    int passed = 0, failed = 0;
    int expected_failures = 0;
    int unexpected_successes = 0;

    auto run = [&](const Result &r, const std::string &name)
    {
        print_result(r, name);
        if (r.ok)
            ++passed;
        else
            ++failed;
    };

    // helper for tests that are expected to fail: treat r.ok == false as a "pass"
    auto run_expected = [&](const Result &r, const std::string &name)
    {
        print_result(r, name + " (expected)");
        if (!r.ok)
        {
            ++passed; // count as pass since failure was expected
            ++expected_failures;
        }
        else
        {
            ++failed; // unexpected success
            ++unexpected_successes;
        }
    };

    // Simple integer equality
    run(eq(1, 1, "int eq"), "int eq");
    run_expected(eq(42, 41, "int mismatch demo"), "int mismatch");

    // Signed/unsigned comparisons
    unsigned u = 5;
    int s = 5;
    run(eq(u, s, "signed/unsigned eq"), "signed/unsigned eq");
    run(gt(10u, 5, "unsigned gt int"), "unsigned gt int");

    // Float approximate comparisons
    run(approx_eq(0.1 + 0.2, 0.3, 1e-6, "float approx"), "float approx (0.1+0.2 vs 0.3)");
    run_expected(approx_eq(1.0000001, 1.0, 1e-8, "too strict tol"), "float approx too strict");

    // Strings and C-strings
    std::string a = "hello";
    const char *b = "hello";
    run(eq(a, std::string(b), "string vs cstring"), "string vs cstring");
    run(ne(std::string("a"), std::string("b"), "string ne"), "string ne");

    // Pointers
    int x = 10;
    int *px = &x;
    int *py = &x;
    int y = 10;
    int *pz = &y;
    run(ptr_eq(px, py, "same pointer"), "pointer same");
    run_expected(ptr_eq(px, pz, "different pointer"), "pointer different");

    // Containers
    std::vector<int> v1 = {1, 2, 3}, v2 = {1, 2, 3}, v3 = {1, 2, 4};
    run(container_eq(v1, v2, "vector equal"), "vector equal");
    run_expected(container_eq(v1, v3, "vector mismatch"), "vector mismatch");

    // Predicate
    run(predicate([](int n)
                  { return n % 2 == 0; }, 4, "is even"),
        "predicate even");
    run_expected(predicate([](int n)
                           { return n % 2 == 0; }, 3, "is even fail"),
                 "predicate even fail");

    // Exceptions
    run(throws<std::runtime_error>([]()
                                   { throw std::runtime_error("boom"); }, "should throw runtime_error"),
        "throws runtime_error");
    run_expected(throws<std::runtime_error>([]() { /* no throw */ }, "should throw but does not"), "throws none");

    // Does not throw
    run(does_not_throw([]()
                       { int z = 1+2; (void)z; }, "no throw"),
        "does_not_throw pass");
    run_expected(does_not_throw([]()
                                { throw std::logic_error("oops"); }, "throws logic_error unexpectedly"),
                 "does_not_throw fail");

    // Additional tests: functions, member pointers, std::function, lambdas

    // function pointer tests
    int (*fp_add)(int, int) = [](int a, int b) -> int
    { return a + b; };
    int (*fp_add2)(int, int) = fp_add;
    int (*fp_sub)(int, int) = [](int a, int b) -> int
    { return a - b; };
    run(ptr_eq((void *)fp_add, (void *)fp_add2, "function pointer same"), "function pointer same");
    run_expected(ptr_eq((void *)fp_add, (void *)fp_sub, "function pointer different"), "function pointer different");

    // member-data and member-function pointers
    struct S
    {
        int v;
        int get() const { return v; }
    };
    S s1{5}, s2{5}, s3{6};
    int S::*md = &S::v;
    run(eq(s1.*md, s2.*md, "member-data value eq"), "member-data value eq");
    run_expected(eq(s1.*md, s3.*md, "member-data value neq"), "member-data value neq (should fail)");

    int (S::*mf)() const = &S::get;
    run(eq((s1.*mf)(), 5, "member-fn call eq"), "member-fn call eq");
    run(ne((s1.*mf)(), (s3.*mf)(), "member-fn call ne"), "member-fn call ne");

    // std::function and lambda capture tests
    std::function<int(int)> f = [](int n)
    { return n * 2; };
    run(eq(f(3), 6, "std::function result"), "std::function result");

    int factor = 3;
    auto lam = [factor](int n)
    { return n * factor; };
    run(eq(lam(4), 12, "lambda capture"), "lambda capture");

    // nullptr test
    int *pn = nullptr;
    run(ptr_eq(pn, nullptr, "nullptr equality"), "nullptr equality");

    // Summary: use tester::Message for consistent formatting and include expected/unexpected counts
    {
        tester::Message sum;
        using namespace tester::colors;
        if (failed == 0 && unexpected_successes == 0)
            sum.SetColor(GREEN) << "Summary: passed=" << passed << " failed=" << failed;
        else
            sum.SetColor(std::string(BG_BRIGHT_RED) + std::string(WHITE)) << "Summary: passed=" << passed << " failed=" << failed;
        sum << "  (expected failures=" << expected_failures << " unexpected_successes=" << unexpected_successes << ")";
        std::cout << "\n"
                  << sum << "\n";
    }
    return failed == 0 ? 0 : 1;
}
