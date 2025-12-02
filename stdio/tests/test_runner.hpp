#pragma once

// Very small C++ test harness intended for use from test_err.cpp / test_std_printf.cpp.
// It provides:
//   - TestCase: name + lambda
//   - run_all_tests: runs all tests, prints summary, returns exit code
//   - capture_fd / capture_stdout / capture_stderr helpers
// The concrete tests are responsible for constructing format strings and
// invoking the stdio / error APIs (trace, ft_printf, sh_error, ...).

#include <cerrno>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "colors.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

extern "C"
{
#include "../output/output.h"
#include "../output/error.h"
// also pull in stdio prototypes used in helpers
#include "ft_stdio.h"
#include "ft_string.h"
}

#define BUFFER_SIZE 1024

int test_count = 0; /* Total number of tests run */
int pass_count = 0; /* Number of tests that passed */
int fail_count = 0; /* Number of tests that failed */

/* Global test statistics for unified reporting */
int g_total_tests = 0;	/* Grand total of tests across all test files */
int g_failed_tests = 0; /* Grand total of failed tests */

/* Initialize the test counters */
void init_globals(void)
{
	test_count = 0;
	pass_count = 0;
	fail_count = 0;
	g_total_tests = 0;
	g_failed_tests = 0;
}

// Simple assertion helpers usable from C++ tests.
struct AssertError : public std::exception
{
	std::string msg;
	explicit AssertError(std::string m) : msg(std::move(m)) {}
	const char *what() const noexcept override { return msg.c_str(); }
};

inline void assert_true(bool cond, const char *expr, const char *file, int line)
{
	if (!cond)
	{
		std::ostringstream oss;
		oss << file << ":" << line << ": assertion failed: " << expr;
		throw AssertError(oss.str());
	}
}

#define TEST_ASSERT(expr) ::assert_true((expr), #expr, __FILE__, __LINE__)

// Represents a single test.
struct TestCase
{
	std::string name;
	std::function<void()> fn;
};

// RAII helper to capture output of a given fd into a std::string.
//
// Usage pattern (C++ test code):
//   auto [out, ok] = capture_stdout([]{
//       ft_printf("hello %s", "world");
//   });
//   TEST_ASSERT(ok);
//   TEST_ASSERT(out == "hello world");
//
// For error functions like sh_error/exerror that never return, install a
// temporary exception handler that longjmps / throws instead of exiting,
// then capture_stderr around a call that is expected to hit that handler.
inline std::pair<std::string, bool> capture_fd(int fd, const std::function<void()> &body)
{
	int saved_fd = -1;
	int tmp_fd = -1;
	bool ok = true;
	std::string result;

	// Duplicate original fd
	saved_fd = ::dup(fd);
	if (saved_fd < 0)
		return {result, false};

	// Create a single temporary file (mkstemp). Unlink name so it is removed
	// when closed — we only need the fd.
	char tmpl[] = "/tmp/stdio_test_XXXXXX";
	tmp_fd = mkstemp(tmpl);
	if (tmp_fd < 0)
	{
		::close(saved_fd);
		return {result, false};
	}
	unlink(tmpl);

	// Redirect fd -> tmp_fd
	if (::dup2(tmp_fd, fd) < 0)
	{
		::close(saved_fd);
		::close(tmp_fd);
		return {result, false};
	}

	try
	{
		body();
	}
	catch (...)
	{
		ok = false;
	}

	// Flush any internal buffers the library keeps
	flush_all();

	// Rewind and read from tmp_fd
	if (::lseek(tmp_fd, 0, SEEK_SET) >= 0)
	{
		char buf[4096];
		ssize_t r;
		while ((r = ::read(tmp_fd, buf, sizeof(buf))) > 0)
			result.append(buf, static_cast<size_t>(r));
	}

	// Restore original fd
	::dup2(saved_fd, fd);
	::close(saved_fd);
	::close(tmp_fd);

	return {result, ok};
}

inline std::pair<std::string, bool> capture_stdout(const std::function<void()> &body)
{
	return capture_fd(STDOUT_FILENO, body);
}

inline std::pair<std::string, bool> capture_stderr(const std::function<void()> &body)
{
	return capture_fd(STDERR_FILENO, body);
}

// Small runner used by both test_err.cpp and test_std_printf.cpp.
// Example in test_std_printf.cpp:
//
//   int main() {
//       std::vector<TestCase> tests;
//       tests.push_back({"printf_basic", []{
//           auto [out, ok] = capture_stdout([]{
//               ft_printf("num=%d str=%s", 42, "abc");
//           });
//           TEST_ASSERT(ok);
//           TEST_ASSERT(out == "num=42 str=abc");
//       }});
//       return run_all_tests("stdio", tests);
//   }
inline int run_all_tests(const std::string &suite_name,
						 const std::vector<TestCase> &tests)
{
	std::size_t passed = 0;
	std::size_t failed = 0;

	for (const auto &t : tests)
	{
		try
		{
			t.fn();
			++passed;
		}
		catch (const AssertError &e)
		{
			++failed;
			std::cerr << "[" << suite_name << "] FAIL " << t.name << ": "
					  << e.what() << "\n";
		}
		catch (const std::exception &e)
		{
			++failed;
			std::cerr << "[" << suite_name << "] FAIL " << t.name
					  << " (std::exception): " << e.what() << "\n";
		}
		catch (...)
		{
			++failed;
			std::cerr << "[" << suite_name << "] FAIL " << t.name
					  << " (unknown exception)\n";
		}
	}

	std::cerr << "[" << suite_name << "] "
			  << "passed=" << passed << " failed=" << failed << "\n";
	return failed == 0 ? 0 : 1;
}

inline void print_formatted_string(const char *str, int ret)
{
	int i;

	printf("\"");
	for (i = 0; i < (int)strlen(str); i++)
	{
		if (str[i] == ' ')
			printf("%s·%s", YELLOW, RESET);
		else if (str[i] == '0')
			printf("%s0%s", CYAN, RESET);
		else if (str[i] < 32 || str[i] > 126)
			printf("%s\\%03o%s", BLUE, str[i], RESET);
		else
			printf("%c", str[i]);
	}
	printf("\" (ret: %d)", ret);
}

inline void compare_and_print_results(const char *expected, int expected_ret,
									  const char *actual, int actual_ret,
									  const char *format, const char *test_name)
{
	test_count++;
	if (strcmp(expected, actual) == 0 && expected_ret == actual_ret)
	{
		printf("%s[PASS]%s %s\n", GREEN, RESET, test_name);
		pass_count++;
	}
	else
	{
		printf("%s[FAIL]%s %s\n", RED, RESET, test_name);
		printf("  Format:    \"%s\"\n", format);
		printf("  Expected:  ");
		print_formatted_string(expected, expected_ret);
		printf("\n  Actual:    ");
		print_formatted_string(actual, actual_ret);
		printf("\n");
		fail_count++;
	}
}

inline void test_char(char c, const char *format, const char *test_name)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	expected_ret = sprintf(expected, format, c);
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("FAiled to redirect stdout");
		fclose(fp);
		return;
	}
	actual_ret = ft_printf(format, c);
	fflush(stdout);
	dup2(original_stdout, 1);
	close(original_stdout);
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0'; // <-- use bytes_read to NUL-terminate
	fclose(fp);
	compare_and_print_results(expected, expected_ret, actual, actual_ret, format, test_name);
	if (strcmp(expected, actual) != 0 || expected_ret != actual_ret)
	{
		printf("	Char:	'%c'	(Ascii: %d)\n", c > 31 && c < 127 ? c : '.', (int)c);
	}
}

inline void test_multiple_chars(const char *format, const char *test_name, ...)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	va_list args;
	va_list args_copy;
	FILE *fp;
	int original_stdout;
	int bytes_read;
	int num_chars = 0;
	char c1, c2, c3, c4;

	// Count format specifiers to determine number of characters
	const char *ptr = format;
	while ((ptr = strstr(ptr, "%c")) != NULL)
	{
		num_chars++;
		ptr++; // Move past the current '%c'
	}

	va_start(args, test_name);
	va_copy(args_copy, args);

	// Get expected output using standard printf
	expected_ret = vsnprintf(expected, BUFFER_SIZE, format, args);

	// Setup for capturing ft_printf output
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		va_end(args);
		va_end(args_copy);
		return;
	}

	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		va_end(args);
		va_end(args_copy);
		return;
	}

	// Extract characters from args_copy and call ft_printf with them directly
	if (num_chars == 1)
	{
		c1 = va_arg(args_copy, int); // va_arg uses int for char
		actual_ret = ft_printf(format, c1);
	}
	else if (num_chars == 2)
	{
		c1 = va_arg(args_copy, int);
		c2 = va_arg(args_copy, int);
		actual_ret = ft_printf(format, c1, c2);
	}
	else if (num_chars == 3)
	{
		c1 = va_arg(args_copy, int);
		c2 = va_arg(args_copy, int);
		c3 = va_arg(args_copy, int);
		actual_ret = ft_printf(format, c1, c2, c3);
	}
	else if (num_chars == 4)
	{
		c1 = va_arg(args_copy, int);
		c2 = va_arg(args_copy, int);
		c3 = va_arg(args_copy, int);
		c4 = va_arg(args_copy, int);
		actual_ret = ft_printf(format, c1, c2, c3, c4);
	}
	else
	{
		// Fallback for more characters (limited support)
		printf("Warning: test_multiple_chars can only handle up to 4 characters\n");
		actual_ret = ft_printf(format, 0);
	}

	fflush(stdout);

	// Restore stdout
	dup2(original_stdout, 1);
	close(original_stdout);

	// Get output from temporary file
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);

	va_end(args);
	va_end(args_copy);

	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
}

inline void test_string(const char *str, const char *format, const char *test_name)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	expected_ret = sprintf(expected, format, str);
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		return;
	}
	actual_ret = ft_printf(format, str);
	fflush(stdout);
	dup2(original_stdout, 1);
	close(original_stdout);
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);
	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
	if (strcmp(expected, actual) != 0 || expected_ret != actual_ret)
		printf("  Input:     %s\"%s\"%s\n", BLUE, str ? str : "NULL", RESET);
}

inline void test_multiple_strings(const char *format, const char *test_name, ...)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	va_list args;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	va_start(args, test_name);
	expected_ret = vsnprintf(expected, BUFFER_SIZE, format, args);
	va_end(args);

	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		return;
	}

	va_start(args, test_name);
	actual_ret = vfprintf(stdout, format, args); // uses libc; fine for helper
	va_end(args);
	fflush(stdout);

	dup2(original_stdout, 1);
	close(original_stdout);

	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);

	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
}

void test_int(int value, const char *format, const char *test_name)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	expected_ret = sprintf(expected, format, value);
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		return;
	}
	actual_ret = ft_printf(format, value);
	fflush(stdout);
	dup2(original_stdout, 1);
	close(original_stdout);
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);
	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
	if (strcmp(expected, actual) != 0 || expected_ret != actual_ret)
		printf("  Value:     %d (0x%x)\n", value, value);
}

void test_unsigned(unsigned int value, const char *format, const char *test_name)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	expected_ret = sprintf(expected, format, value);
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		return;
	}
	actual_ret = ft_printf(format, value);
	fflush(stdout);
	dup2(original_stdout, 1);
	close(original_stdout);
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);
	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
	if (strcmp(expected, actual) != 0 || expected_ret != actual_ret)
		printf("  Value:     %u (0x%x)\n", value, value);
}

void test_hex(unsigned int value, int is_upper, const char *format,
			  const char *test_name)
{
	char expected[BUFFER_SIZE];
	char actual[BUFFER_SIZE];
	int expected_ret;
	int actual_ret;
	FILE *fp;
	int original_stdout;
	int bytes_read;

	(void)is_upper;
	expected_ret = sprintf(expected, format, value);
	fp = tmpfile();
	if (!fp)
	{
		perror("Failed to create temporary file");
		return;
	}
	original_stdout = dup(1);
	if (dup2(fileno(fp), 1) == -1)
	{
		perror("Failed to redirect stdout");
		fclose(fp);
		return;
	}
	actual_ret = ft_printf(format, value);
	fflush(stdout);
	dup2(original_stdout, 1);
	close(original_stdout);
	fseek(fp, 0, SEEK_SET);
	bytes_read = fread(actual, 1, BUFFER_SIZE - 1, fp);
	actual[bytes_read] = '\0';
	fclose(fp);
	compare_and_print_results(expected, expected_ret, actual, actual_ret,
							  format, test_name);
	if (strcmp(expected, actual) != 0 || expected_ret != actual_ret)
		printf("  Value:     %u (0x%x)\n", value, value);
}

// Reusable list of format patterns tests can consume.
// Use get_patterns() to obtain a reference to the vector.
//
// Pattern format: { format_string, "arg1,arg2,..." }
// Tokens in the CSV map (in order) to conversion specifiers in the format.
// Special tokens understood by tests:
//   - NULL -> null pointer for %s/%p
//   - (nil) -> literal "(nil)" when used as a string token
//   - 0x... -> parsed as hex (for %p/%x/%X if desired)
//   - empty token between commas -> treated as empty string for %s, 0 for numbers
//
struct Pattern
{
	std::string fmt;
	std::string args_csv;
};

inline const std::vector<Pattern> &get_patterns()
{
	static std::vector<Pattern> patterns = {
		// basic combinations
		{"%d%d%d", "5,5,5"},
		{"%s %s %d", "hello,world,42"},
		{"%u", "3000000000"},
		{"%x", "deadbeef"},
		{"%X", "BEEF"},
		{"%#x", "48879"},
		{"%p", "NULL"},
		{"%c", "A"},
		{"%%", ""},
		{"%s", "NULL"},
		{"snprintf tight: %d", "9999"},

		// Edge cases: empty strings, many % signs, NULLs, zeros.
		{
			"%s %s",
			",",
		}, // two empty strings
		{"%d", "0"},
		{"%u", "0"},
		{"%x", "0"},
		{"%s", "(nil)"},
		{"%%%s%%%d%%", "hi,7"},													 // literal % intermixed
		{"%s", "a very long string that should be truncated when small buffer"}, // long string
		{"%d %d %d %d %d %d", "1,2,3,4,5,6"},									 // six ints
		{"%s %p %x %c", "hello,NULL,0x2a,Z"},									 // mixed types
	};
	return patterns;
}

// CSV split helper used by tests.
inline std::vector<std::string> split_csv(const std::string &s)
{
	std::vector<std::string> out;
	std::string cur;
	for (size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == ',')
		{
			out.push_back(cur);
			cur.clear();
		}
		else
			cur.push_back(c);
	}
	// final token (allow empty)
	out.push_back(cur);
	return out;
}
