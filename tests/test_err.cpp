#include "test_runner.hpp"

extern "C"
{
#include "ft_stdio.h"
}

// Global to record exception code set by exraise/onint/sh_error/exerror.
static int g_last_exception = -1;

// Custom handler that records exception and throws C++ exception to unwind.
static void test_exception_handler(int e)
{
    g_last_exception = e;
    throw std::runtime_error("test-exception");
}

static void setup_handler()
{
    g_last_exception = -1;
    set_exception_handler(&test_exception_handler);
}

static void add_warn_tests(std::vector<TestCase> &tests)
{
    // sh_warnx should write prefix "sh: <lineno>: " and message, no exit.
    tests.push_back({"sh_warnx_basic", []()
                     {
                         setup_handler(); // handler should not be used here.
                         auto [out, ok] = capture_stderr([]()
                                                         { sh_warnx("warn %s %d", "msg", 7); });
                         TEST_ASSERT(ok);
                         // rough check: contains "warn msg 7"
                         TEST_ASSERT(out.find("warn msg 7") != std::string::npos);
                         TEST_ASSERT(g_last_exception == -1);
                     }});

    // ft_warn and ft_error are aliases printing warning-like messages.
    tests.push_back({"ft_warn_and_error_basic", []()
                     {
                         setup_handler();
                         auto [out1, ok1] = capture_stderr([]()
                                                           { ft_warn("ft_warn %d", 1); });
                         auto [out2, ok2] = capture_stderr([]()
                                                           { ft_error("ft_error %d", 2); });
                         TEST_ASSERT(ok1 && ok2);
                         TEST_ASSERT(out1.find("ft_warn 1") != std::string::npos);
                         TEST_ASSERT(out2.find("ft_error 2") != std::string::npos);
                         TEST_ASSERT(g_last_exception == -1);
                     }});
}

static void add_error_noreturn_tests(std::vector<TestCase> &tests)
{
    // sh_error should print and then call handler with EXERROR.
    tests.push_back({"sh_error_raises_EXERROR", []()
                     {
                         setup_handler();
                         std::string captured;
                         bool ok = false;

                         auto [out, success] = capture_stderr([&]()
                                                              {
            try {
                sh_error("fatal %s", "oops");
            } catch (const std::runtime_error &) {
                ok = true;
            } });
                         captured = out;
                         TEST_ASSERT(success); // write path OK
                         TEST_ASSERT(ok);      // handler threw
                         TEST_ASSERT(g_last_exception == EXERROR);
                         TEST_ASSERT(captured.find("fatal oops") != std::string::npos);
                     }});

    // exerror with custom code.
    tests.push_back({"exerror_custom_code", []()
                     {
                         setup_handler();
                         std::string captured;
                         bool ok = false;
                         int custom = 42;

                         auto [out, success] = capture_stderr([&]()
                                                              {
            try {
                exerror(custom, "fatal %s %d", "err", 123);
            } catch (const std::runtime_error &) {
                ok = true;
            } });
                         captured = out;
                         TEST_ASSERT(success);
                         TEST_ASSERT(ok);
                         TEST_ASSERT(g_last_exception == custom);
                         TEST_ASSERT(captured.find("fatal err 123") != std::string::npos);
                     }});

    // exraise called directly.
    tests.push_back({"exraise_direct", []()
                     {
                         setup_handler();
                         bool ok = false;
                         int code = 7;

                         try
                         {
                             exraise(code);
                         }
                         catch (const std::runtime_error &)
                         {
                             ok = true;
                         }
                         TEST_ASSERT(ok);
                         TEST_ASSERT(g_last_exception == code);
                     }});

    // onint should raise EXINT
    tests.push_back({"onint_raises_EXINT", []()
                     {
                         setup_handler();
                         bool ok = false;
                         try
                         {
                             onint();
                         }
                         catch (const std::runtime_error &)
                         {
                             ok = true;
                         }
                         TEST_ASSERT(ok);
                         TEST_ASSERT(g_last_exception == EXINT);
                     }});
}

static void add_errmsg_tests(std::vector<TestCase> &tests)
{
    tests.push_back({"errmsg_basic", []()
                     {
                         const char *m1 = errmsg(ENOENT, E_OPEN);
                         const char *m2 = errmsg(ENOENT, E_CREAT);
                         const char *m3 = errmsg(ENOENT, 0);
                         const char *m4 = errmsg(EPERM, 0);

                         TEST_ASSERT(std::string(m1) == "No such file");
                         TEST_ASSERT(std::string(m2) == "Directory nonexistent");
                         TEST_ASSERT(std::string(m3) == "not found");
                         TEST_ASSERT(std::string(m4) == std::string(strerror(EPERM)));
                     }});
}

int main()
{
    std::vector<TestCase> tests;
    add_warn_tests(tests);
    add_error_noreturn_tests(tests);
    add_errmsg_tests(tests);
    return run_all_tests("stdio_error", tests);
}
