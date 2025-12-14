// #include <iostream>
// #include <string>
// #include <vector>
// #include <cstdarg>

// extern "C"
// {
// #include "output.h"
// #include "error.h"
// #include "format.h"
// #include "ft_stdio.h"
// #include "ft_string.h"
// #include "ft_stdlib.h"
// #include <stdarg.h>
// }
// #include "test_runner.hpp" // use enhanced test helpers

// /*
//  * Helper: section separator.
//  */
// void sep(const char *title)
// {
// 	std::cout << "\n==== " << title << " ====\n";
// }

// /*
//  * Demonstrate trace / tracev / trputs / trputc + indent.
//  */
// void demo_tracing()
// {
// 	sep("TRACE / TRACEV / TRPUTS / INDENT");

// 	set_debug(1);	 // enable tracing
// 	set_trace_fd(2); // stderr

// 	std::cout << "(trace outputs go to stderr)\n";

// 	trace("simple trace: %s %d\n", "hello", 42);

// 	// tracev demonstration: build a va_list properly via a small helper lambda
// 	auto tracev_helper = [](const char *f, ...)
// 	{
// 		va_list va;
// 		va_start(va, f);
// 		tracev(f, va);
// 		va_end(va);
// 	};
// 	tracev_helper("tracev: %s %u 0x%x\n", "world", 123u, 0xdeadbeefu);

// 	trputs("trputs plain string\n");
// 	indent(3, (char *)"[LAST]", get_trace_fd());
// 	trputc('\n');

// 	set_debug(0);
// }

// /*
//  * Demonstrate outmem and out1fmt.
//  */
// void demo_outmem_out1fmt()
// {
// 	sep("OUTMEM / OUT1FMT");

// 	t_out_ctx *ctx = get_outs();
// 	t_out *dst = ctx->out1;

// 	// initialize buffer for manual outmem
// 	if (!dst->buf)
// 	{
// 		dst->buf_size = OUTBUFSIZE;
// 		dst->buf = (char *)malloc(dst->buf_size);
// 		dst->nextc = dst->buf;
// 		dst->end = dst->buf + dst->buf_size;
// 		dst->fd = STDOUT_FILENO;
// 		dst->flags = 0;
// 	}

// 	const char *msg = "outmem: [A][B][C]\n";
// 	outmem(msg, ft_strlen(msg), dst);
// 	flushout(dst);

// 	out1fmt("out1fmt: %s %d 0x%x\n", "hello", 42, 0xdeadbeef);
// }

// /*
//  * Demonstrate fmtstr with various cases.
//  */
// void demo_fmtstr()
// {
// 	sep("FMTSTR");

// 	char buf[64];

// 	fmtstr(buf, sizeof(buf), "plain string\n");
// 	std::cout << "fmtstr #1: " << buf;

// 	fmtstr(buf, sizeof(buf), "int=%d str=%s\n", 123, "abc");
// 	std::cout << "fmtstr #2: " << buf;

// 	// truncated
// 	fmtstr(buf, 8, "very long: %d %s\n", 99999, "zzzz");
// 	std::cout << "fmtstr #3 (truncated cap=8): " << std::string(buf) << "\n";
// }

// /*
//  * Demonstrate ft_write wrapper.
//  */
// void demo_ft_write()
// {
// 	sep("FT_WRITE");

// 	const char *s = "ft_write to stdout\n";
// 	if (ft_write(STDOUT_FILENO, s, ft_strlen(s)) != 0)
// 		std::cerr << "ft_write: error\n";
// }

// /*
//  * Demonstrate log_print (with t_state_fd).
//  */
// void demo_log_print()
// {
// 	sep("LOG_PRINT");

// 	t_state_fd *st = get_state_fd();
// 	st->fd = STDERR_FILENO;
// 	st->state = ST_INFO_BASE;
// 	log_print(st, "demo_log_print", "info message %d", 1);
// 	st->state = ST_WARNING_BASE;
// 	log_print(st, "demo_log_print", "warning %s", "something odd");
// 	st->state = ST_ERR_BASE;
// 	log_print(st, "demo_log_print", "error code=%d", -1);
// 	st->state = ST_OK;
// 	log_print(st, "demo_log_print", "operation ok");
// }

// /* forward declaration so demo_printf_family can call it */
// //static void run_all_variants_local(const char *fmt, const char *s1, const char *s2, int i);

// /* small helper: extract conversion specifiers from a format string */
// static std::vector<char> extract_specifiers(const std::string &fmt)
// {
// 	std::vector<char> specs;
// 	size_t i = 0;
// 	while (i < fmt.size())
// 	{
// 		if (fmt[i] == '%')
// 		{
// 			++i;
// 			if (i < fmt.size() && fmt[i] == '%')
// 			{
// 				++i;
// 				continue; // escaped '%'
// 			}
// 			// skip flags / width / precision roughly
// 			while (i < fmt.size() && strchr("-+0 #", fmt[i]))
// 				++i;
// 			while (i < fmt.size() && isdigit((unsigned char)fmt[i]))
// 				++i;
// 			if (i < fmt.size() && fmt[i] == '.')
// 			{
// 				++i;
// 				while (i < fmt.size() && isdigit((unsigned char)fmt[i]))
// 					++i;
// 			}
// 			// length (ignore)
// 			if (i < fmt.size() && (fmt[i] == 'l' || fmt[i] == 'h'))
// 				++i;
// 			if (i < fmt.size())
// 			{
// 				specs.push_back(fmt[i]);
// 				++i;
// 			}
// 		}
// 		else
// 			++i;
// 	}
// 	return specs;
// }

// /* tiny utility for trimming CSV tokens (reuse from tests) */
// static std::string trim_token(const std::string &s)
// {
// 	size_t a = 0, b = s.size();
// 	while (a < b && std::isspace((unsigned char)s[a]))
// 		++a;
// 	while (b > a && std::isspace((unsigned char)s[b - 1]))
// 		--b;
// 	return s.substr(a, b - a);
// }

// /*
//  * Convert a CSV token to a basic C value, based on a specifier.
//  * Only supports what we actually use in patterns: d/i/u/x/X/s/c/p.
//  */
// static void token_to_basic(const std::string &raw, char spec,
// 						   int &out_i, unsigned int &out_u,
// 						   const char *&out_s, void *&out_p, char &out_c)
// {
// 	std::string t = trim_token(raw);
// 	out_i = 0;
// 	out_u = 0;
// 	out_s = nullptr;
// 	out_p = nullptr;
// 	out_c = 0;

// 	if (t == "NULL")
// 	{
// 		out_s = nullptr;
// 		out_p = nullptr;
// 		return;
// 	}
// 	if (t == "(nil)")
// 	{
// 		out_s = t.c_str(); // used only for %s patterns
// 		return;
// 	}

// 	switch (spec)
// 	{
// 	case 'd':
// 	case 'i':
// 		out_i = t.empty() ? 0 : (int)strtol(t.c_str(), nullptr, 0);
// 		break;
// 	case 'u':
// 	case 'x':
// 	case 'X':
// 		out_u = t.empty() ? 0u : (unsigned int)strtoul(t.c_str(), nullptr, 0);
// 		break;
// 	case 'p':
// 	{
// 		unsigned long v = t.empty() ? 0UL : strtoul(t.c_str(), nullptr, 0);
// 		out_p = (void *)v;
// 		break;
// 	}
// 	case 'c':
// 		out_c = t.empty() ? '\0' : t[0];
// 		break;
// 	case 's':
// 	default:
// 		out_s = t.c_str();
// 		break;
// 	}
// }

// /*
//  * Apply one pattern:
//  *   - Build an expected string with snprintf
//  *   - Compare fmtstr() vs that expected string
//  *   - Call ft_printf once (just to ensure no crash and some output)
//  * Supports up to 3 arguments; extra args in patterns will be reported as "skipped".
//  */
// static void apply_pattern_once(std::size_t idx, const Pattern &p)
// {
// 	std::vector<std::string> tokens = split_csv(p.args_csv);
// 	std::vector<char> specs = extract_specifiers(p.fmt);

// 	// Normalize token count for up to 3 args
// 	if (tokens.size() < specs.size())
// 		tokens.resize(specs.size());

// 	std::cout << "pattern " << idx << ": \"" << p.fmt << "\" args=\"" << p.args_csv << "\"\n";

// 	if (specs.size() == 0)
// 	{
// 		// no args
// 		char expected[256];
// 		ft_memset(expected, 0, sizeof(expected));
// 		snprintf(expected, sizeof(expected), "%s", ""); // just empty check

// 		char buf[256];
// 		ft_memset(buf, 0, sizeof(buf));
// 		int ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str());
// 		std::cout << "  fmtstr(\"" << p.fmt << "\") => \"" << buf << "\" (ret=" << ret_fmt << ")\n";
// 		auto [out, ok] = capture_stdout([&]()
// 										{ ft_printf(p.fmt.c_str()); });
// 		std::cout << "  ft_printf stdout: \"" << out << "\" (ok=" << ok << ")\n";
// 		return;
// 	}

// 	if (specs.size() > 3)
// 	{
// 		std::cout << "  [SKIP] more than 3 specifiers — manual inspection only\n";
// 		return;
// 	}

// 	// prepare typed values for up to 3 args
// 	int i1 = 0, i2 = 0, i3 = 0;
// 	unsigned int u1 = 0, u2 = 0, u3 = 0;
// 	const char *s1 = nullptr, *s2 = nullptr, *s3 = nullptr;
// 	void *p1 = nullptr, *p2 = nullptr, *p3 = nullptr;
// 	char c1 = 0, c2 = 0, c3 = 0;

// 	for (std::size_t k = 0; k < specs.size() && k < 3; ++k)
// 	{
// 		switch (k)
// 		{
// 		case 0:
// 			token_to_basic(tokens[0], specs[0], i1, u1, s1, p1, c1);
// 			break;
// 		case 1:
// 			token_to_basic(tokens[1], specs[1], i2, u2, s2, p2, c2);
// 			break;
// 		case 2:
// 			token_to_basic(tokens[2], specs[2], i3, u3, s3, p3, c3);
// 			break;
// 		}
// 	}

// 	// Build expected with snprintf (up to 3 args)
// 	char expected[256];
// 	ft_memset(expected, 0, sizeof(expected));

// 	// We branch by spec count and very basic type classification; this is for the demo only.
// 	if (specs.size() == 1)
// 	{
// 		char spec = specs[0];
// 		if (spec == 's')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), s1);
// 		else if (spec == 'd' || spec == 'i')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), i1);
// 		else if (spec == 'u' || spec == 'x' || spec == 'X')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), u1);
// 		else if (spec == 'c')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), c1);
// 		else if (spec == 'p')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), p1);
// 		else
// 			snprintf(expected, sizeof(expected), "%s", "(unsupported spec)");
// 	}
// 	else if (specs.size() == 2)
// 	{
// 		// This only handles a couple of common shapes, e.g. "%s %s", "%%%s%%%d%%"
// 		if (specs[0] == 's' && specs[1] == 's')
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), s1, s2);
// 		else if (specs[0] == 's' && (specs[1] == 'd' || specs[1] == 'i'))
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), s1, i2);
// 		else
// 			snprintf(expected, sizeof(expected), "%s", "(unsupported 2-arg pattern)");
// 	}
// 	else if (specs.size() == 3)
// 	{
// 		// e.g. "%s %s %d"
// 		if (specs[0] == 's' && specs[1] == 's' && (specs[2] == 'd' || specs[2] == 'i'))
// 			snprintf(expected, sizeof(expected), p.fmt.c_str(), s1, s2, i3);
// 		else
// 			snprintf(expected, sizeof(expected), "%s", "(unsupported 3-arg pattern)");
// 	}

// 	// fmtstr vs expected
// 	char buf[256];
// 	ft_memset(buf, 0, sizeof(buf));
// 	int ret_fmt = 0;
// 	if (specs.size() == 1)
// 	{
// 		char spec = specs[0];
// 		if (spec == 's')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), s1);
// 		else if (spec == 'd' || spec == 'i')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), i1);
// 		else if (spec == 'u' || spec == 'x' || spec == 'X')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), u1);
// 		else if (spec == 'c')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), c1);
// 		else if (spec == 'p')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), p1);
// 		else
// 			ret_fmt = fmtstr(buf, sizeof(buf), "%s", "(unsupported spec)");
// 	}
// 	else if (specs.size() == 2)
// 	{
// 		if (specs[0] == 's' && specs[1] == 's')
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), s1, s2);
// 		else if (specs[0] == 's' && (specs[1] == 'd' || specs[1] == 'i'))
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), s1, i2);
// 		else
// 			ret_fmt = fmtstr(buf, sizeof(buf), "%s", "(unsupported 2-arg)");
// 	}
// 	else if (specs.size() == 3)
// 	{
// 		if (specs[0] == 's' && specs[1] == 's' && (specs[2] == 'd' || specs[2] == 'i'))
// 			ret_fmt = fmtstr(buf, sizeof(buf), p.fmt.c_str(), s1, s2, i3);
// 		else
// 			ret_fmt = fmtstr(buf, sizeof(buf), "%s", "(unsupported 3-arg)");
// 	}

// 	std::cout << "  expected (snprintf): \"" << expected << "\"\n";
// 	std::cout << "  fmtstr:             \"" << buf << "\" (ret=" << ret_fmt << ")\n";

// 	// ft_printf: just ensure it runs and prints something
// 	auto [out, ok] = capture_stdout([&]()
// 									{
// 		if (specs.size() == 1)
// 		{
// 			char spec = specs[0];
// 			if (spec == 's')      ft_printf(p.fmt.c_str(), s1);
// 			else if (spec == 'd' || spec == 'i') ft_printf(p.fmt.c_str(), i1);
// 			else if (spec == 'u' || spec == 'x' || spec == 'X') ft_printf(p.fmt.c_str(), u1);
// 			else if (spec == 'c') ft_printf(p.fmt.c_str(), c1);
// 			else if (spec == 'p') ft_printf(p.fmt.c_str(), p1);
// 			else                  ft_printf("%s", "(unsupported spec)");
// 		}
// 		else if (specs.size() == 2)
// 		{
// 			if (specs[0] == 's' && specs[1] == 's')
// 				ft_printf(p.fmt.c_str(), s1, s2);
// 			else if (specs[0] == 's' && (specs[1] == 'd' || specs[1] == 'i'))
// 				ft_printf(p.fmt.c_str(), s1, i2);
// 			else
// 				ft_printf("%s", "(unsupported 2-arg)");
// 		}
// 		else if (specs.size() == 3)
// 		{
// 			if (specs[0] == 's' && specs[1] == 's' && (specs[2] == 'd' || specs[2] == 'i'))
// 				ft_printf(p.fmt.c_str(), s1, s2, i3);
// 			else
// 				ft_printf("%s", "(unsupported 3-arg)");
// 		} });
// 	std::cout << "  ft_printf stdout:   \"" << out << "\" (ok=" << ok << ")\n";
// }

// /*
//  * Demonstrate printf-family using centralized patterns from get_patterns().
//  * For each pattern, we compare fmtstr vs libc snprintf, and sanity-check ft_printf.
//  */
// void demo_printf_family()
// {
// 	sep("PRINTF FAMILY (pattern-driven demo)");

// 	init_globals();

// 	const auto &patterns = get_patterns();
// 	for (std::size_t idx = 0; idx < patterns.size(); ++idx)
// 	{
// 		sep(("pattern " + std::to_string(idx)).c_str());
// 		apply_pattern_once(idx, patterns[idx]);
// 	}

// 	std::cout << "\nManual printf-family pattern-driven run complete.\n";
// }

// // /*
// //  * Add a local helper that performs the same checks as test_std_printf::run_all_variants
// //  */
// // void run_all_variants_local(const char *fmt, const char *s1, const char *s2, int i)
// // {
// // 	// Build reference using ft_sprintf (same engine)
// // 	char ref[512];
// // 	char buf_sprintf[512];
// // 	char buf_snprintf[512];
// // 	char buf_fmtstr[512];
// // 	char *dyn = NULL;

// // 	ft_memset(ref, 0, sizeof(ref));
// // 	ft_sprintf(ref, fmt, s1, s2, i);

// // 	// 1) ft_printf -> stdout
// // 	auto [out_printf, ok_printf] = capture_stdout([&]()
// // 												  { ft_printf(fmt, s1, s2, i); });
// // 	if (!ok_printf)
// // 		throw AssertError("capture_stdout failed for ft_printf");

// // 	// 2) ft_dprintf to STDERR_FILENO: should match ft_printf output.
// // 	auto [out_dprintf, ok_dprintf] = capture_stderr([&]()
// // 													{ ft_dprintf(STDERR_FILENO, fmt, s1, s2, i); });
// // 	if (!ok_dprintf)
// // 		throw AssertError("capture_stderr failed for ft_dprintf");
// // 	if (out_dprintf != out_printf)
// // 		throw AssertError(std::string("ft_dprintf != ft_printf for fmt: ") + fmt);

// // 	// 3) ft_sprintf to big buffer: must equal ref.
// // 	ft_memset(buf_sprintf, 0, sizeof(buf_sprintf));
// // 	int ret_sprintf = ft_sprintf(buf_sprintf, fmt, s1, s2, i);
// // 	if (std::string(buf_sprintf) != std::string(ref))
// // 		throw AssertError(std::string("ft_sprintf output mismatch for fmt: ") + fmt);
// // 	if (ret_sprintf != (int)std::string(ref).size())
// // 		throw AssertError("ft_sprintf return value mismatch");

// // 	// 4) ft_snprintf with ample capacity: must equal ref.
// // 	ft_memset(buf_snprintf, 0, sizeof(buf_snprintf));
// // 	int ret_snprintf = ft_snprintf(buf_snprintf, sizeof(buf_snprintf), fmt, s1, s2, i);
// // 	if (std::string(buf_snprintf) != std::string(ref))
// // 		throw AssertError(std::string("ft_snprintf (ample) mismatch for fmt: ") + fmt);
// // 	if (ret_snprintf != (int)std::string(ref).size())
// // 		throw AssertError("ft_snprintf return value mismatch");

// // 	// 5) ft_snprintf tight capacity: prefix + NUL termination.
// // 	char small[8];
// // 	ft_memset(small, 'X', sizeof(small));
// // 	int ret_small = ft_snprintf(small, sizeof(small), fmt, s1, s2, i);
// // 	if (small[sizeof(small) - 1] != '\0')
// // 		throw AssertError("ft_snprintf did not NUL-terminate small buffer");
// // 	std::string prefix = std::string(ref).substr(0, sizeof(small) - 1);
// // 	if (std::string(small) != prefix)
// // 		throw AssertError("ft_snprintf small-buffer prefix mismatch");
// // 	if (ret_small < (int)prefix.size())
// // 		throw AssertError("ft_snprintf small return too small");

// // 	// 6) ft_aprintf against ref.
// // 	int ret_aprintf = ft_aprintf(&dyn, fmt, s1, s2, i);
// // 	if (ret_aprintf != (int)std::string(ref).size())
// // 	{
// // 		if (dyn)
// // 			free(dyn);
// // 		throw AssertError("ft_aprintf return mismatch");
// // 	}
// // 	if (dyn == NULL || std::string(dyn) != std::string(ref))
// // 	{
// // 		if (dyn)
// // 			free(dyn);
// // 		throw AssertError("ft_aprintf content mismatch");
// // 	}
// // 	free(dyn);

// // 	// 7) fmtstr helper against ref.
// // 	ft_memset(buf_fmtstr, 0, sizeof(buf_fmtstr));
// // 	int ret_fmtstr = fmtstr(buf_fmtstr, sizeof(buf_fmtstr), fmt, s1, s2, i);
// // 	if (std::string(buf_fmtstr) != std::string(ref))
// // 		throw AssertError("fmtstr output mismatch");
// // 	if (ret_fmtstr != (int)std::string(ref).size())
// // 		throw AssertError("fmtstr return value mismatch");
// // }

// /*
//  * Entry point: run all manual demos.
//  */
// int main()
// {
// 	std::cout << "Manual stdio demo (visual inspection)\n";

// 	// demo_tracing();
// 	// demo_outmem_out1fmt();
// 	// demo_fmtstr();
// 	// demo_ft_write();
// 	// demo_log_print();
// 	demo_printf_family(); // only exercise this one for now
// 	// demo_flush();
// 	// demo_errors();

// 	std::cout << "\nDone. Check both stdout and stderr for output.\n";
// 	return 0;
// }
