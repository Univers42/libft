#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// C API from assert.cpp
int c_assert_int_eq(int a, int b, const char *name);
int c_assert_uint_eq(unsigned a, unsigned b, const char *name);
int c_assert_double_approx(double a, double b, double tol, const char *name);
int c_assert_str_eq(const char *a, const char *b, const char *name);
int c_assert_memcmp(const void *a, const void *b, size_t n, const char *name);
int c_assert_ptr_eq(const void *a, const void *b, const char *name);

// new C formatting helpers
void c_print_expected(const char *name);
void c_print_unexpected_pass(const char *name);

/* Helper: normal run — increments passed or failed */
static void run(int result, const char *name, int *passed, int *failed)
{
    if (result)
        ++(*passed);
    else
        ++(*failed);
}

/* Helper: expected failures — if result == 0 (failure) it's expected and counted as pass.
   If result == 1 it's an unexpected success. */
static void run_expected(int result, const char *name, int *passed, int *failed, int *expected_failures, int *unexpected_successes)
{
    if (result)
    {
        /* unexpected success */
        ++(*failed);
        ++(*unexpected_successes);
        c_print_unexpected_pass(name);
    }
    else
    {
        /* expected failure counts as a pass for the suite */
        ++(*passed);
        ++(*expected_failures);
        c_print_expected(name);
    }
}

int main(void)
{
    int passed = 0, failed = 0;
    int expected_failures = 0, unexpected_successes = 0;
    int r;

    // integer tests
    r = c_assert_int_eq(1, 1, "int eq");
    run(r, "int eq", &passed, &failed);
    r = c_assert_int_eq(42, 41, "int mismatch");
    run_expected(r, "int mismatch", &passed, &failed, &expected_failures, &unexpected_successes); // expected

    // signed/unsigned comparisons
    r = c_assert_uint_eq((unsigned)5, (unsigned)5, "unsigned eq");
    run(r, "unsigned eq", &passed, &failed);

    // double approx
    r = c_assert_double_approx(0.1 + 0.2, 0.3, 1e-6, "double approx");
    run(r, "double approx", &passed, &failed);
    r = c_assert_double_approx(1.0000001, 1.0, 1e-8, "double approx too strict");
    run_expected(r, "double approx too strict", &passed, &failed, &expected_failures, &unexpected_successes); // expected

    // strings
    r = c_assert_str_eq("hello", "hello", "string eq");
    run(r, "string eq", &passed, &failed);
    r = c_assert_str_eq("a", "b", "string ne");
    run_expected(r, "string ne", &passed, &failed, &expected_failures, &unexpected_successes); // expected

    // memcmp (arrays)
    int v1[] = {1, 2, 3};
    int v2[] = {1, 2, 3};
    int v3[] = {1, 2, 4};
    r = c_assert_memcmp(v1, v2, sizeof(v1), "array equal");
    run(r, "array equal", &passed, &failed);
    r = c_assert_memcmp(v1, v3, sizeof(v1), "array mismatch");
    run_expected(r, "array mismatch", &passed, &failed, &expected_failures, &unexpected_successes); // expected

    // pointers
    int x = 10;
    int y = 10;
    int *px = &x;
    int *py = &x;
    int *pz = &y;
    r = c_assert_ptr_eq(px, py, "pointer same");
    run(r, "pointer same", &passed, &failed);
    r = c_assert_ptr_eq(px, pz, "pointer different");
    run_expected(r, "pointer different", &passed, &failed, &expected_failures, &unexpected_successes); // expected

    // summary
    printf("\nSummary: passed=%d failed=%d  (expected failures=%d unexpected_successes=%d)\n",
           passed, failed, expected_failures, unexpected_successes);

    // exit non-zero only if there were unexpected failures (failed > expected_failures)
    int unexpected_failures = failed - expected_failures;
    return (unexpected_failures > 0 || unexpected_successes > 0) ? 1 : 0;
}
