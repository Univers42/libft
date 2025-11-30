#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cstddef>
#ifdef __cplusplus
extern "C"
{
#endif

/* include C prototypes for the functions under test */
#include "ft_ctype.h"
#include <ctype.h>

#ifdef __cplusplus
}
#endif

/* Minimal portable test harness types and helpers (match previous expectations) */
#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		TYPE_BOOL = 0,
		TYPE_INT = 1
	} e_type;

	typedef int (*int_fn_t)(int);

	/* simple wrapper union to mimic original layout used by tests */
	typedef union
	{
		int_fn_t int_fn;
		/* other fn types could be added if needed */
	} t_fn_group;

	typedef struct
	{
		const char *name;
		t_fn_group std_fn;
		t_fn_group ft_fn;
		int param_count;
		e_type type;
	} t_fn_pair;

	typedef struct s_suite
	{
		size_t tests;
		size_t fails;
	} s_suite, t_suite;

	/* helper that test code expected (ft_snprintf) */
	int ft_snprintf(char *dst, size_t cap, const char *fmt, ...)
	{
		va_list ap;
		int r;
		va_start(ap, fmt);
		r = vsnprintf(dst, cap, fmt, ap);
		va_end(ap);
		return r;
	}

	/* minimal suite management */
	t_suite *suite_create(const char *name)
	{
		(void)name;
		t_suite *s = (t_suite *)malloc(sizeof(t_suite));
		if (!s)
			return NULL;
		s->tests = 0;
		s->fails = 0;
		return s;
	}

	void suite_print_summary(const t_suite *s)
	{
		printf("SUMMARY: %zu tests, %zu failures\n", s ? s->tests : 0, s ? s->fails : 0);
	}

	void suite_destroy(t_suite *s)
	{
		if (!s)
			return;
		free(s);
	}

	/* run_var_test: compare two values (ints) and update suite state */
	void run_var_test(t_suite *suite, const char *name,
					  const void *ft_val, const void *std_val,
					  e_type type, size_t bytes)
	{
		(void)bytes;
		int mismatch = 0;
		if (type == TYPE_BOOL)
		{
			int a = !!*(const int *)ft_val;
			int b = !!*(const int *)std_val;
			mismatch = (a != b);
		}
		else /* TYPE_INT */
		{
			int a = *(const int *)ft_val;
			int b = *(const int *)std_val;
			mismatch = (a != b);
		}
		suite->tests++;
		if (mismatch)
		{
			suite->fails++;
			printf("FAILED: %s (expected %d, got %d)\n",
				   name,
				   *(const int *)std_val,
				   *(const int *)ft_val);
		}
	}

#ifdef __cplusplus
}
#endif

/* Original test logic (uses ctype functions declared in ft_ctype.h) */
static t_fn_pair *ctype_pairs(size_t *count)
{
	static t_fn_pair pairs[] = {
		{"isdigit", {.int_fn = isdigit}, {.int_fn = ft_isdigit}, 1, TYPE_BOOL},
		{"isalnum", {.int_fn = isalnum}, {.int_fn = ft_isalnum}, 1, TYPE_BOOL},
		{"isalpha", {.int_fn = isalpha}, {.int_fn = ft_isalpha}, 1, TYPE_BOOL},
		{"isblank", {.int_fn = isblank}, {.int_fn = ft_isblank}, 1, TYPE_BOOL},
		{"isgraph", {.int_fn = isgraph}, {.int_fn = ft_isgraph}, 1, TYPE_BOOL},
		{"islower", {.int_fn = islower}, {.int_fn = ft_islower}, 1, TYPE_BOOL},
		{"isupper", {.int_fn = isupper}, {.int_fn = ft_isupper}, 1, TYPE_BOOL},
		{"isprint", {.int_fn = isprint}, {.int_fn = ft_isprint}, 1, TYPE_BOOL},
		{"isspace", {.int_fn = isspace}, {.int_fn = ft_isspace}, 1, TYPE_BOOL},
		{"isxdigit", {.int_fn = isxdigit}, {.int_fn = ft_isxdigit}, 1, TYPE_BOOL},
		{"tolower", {.int_fn = tolower}, {.int_fn = ft_tolower}, 1, TYPE_INT},
		{"toupper", {.int_fn = toupper}, {.int_fn = ft_toupper}, 1, TYPE_INT},
	};
	if (count)
		*count = sizeof(pairs) / sizeof(*pairs);
	return pairs;
}

static void test_ctype_cmp(t_suite *suite)
{
	size_t npairs, i, c;
	t_fn_pair *pairs = ctype_pairs(&npairs);
	char namebuf[64];

	/* Read optional runtime controls */
	const char *env_max = getenv("TEST_MAX");
	size_t max_c = 256;
	if (env_max)
	{
		long v = strtol(env_max, NULL, 10);
		if (v > 0 && v <= 256)
			max_c = (size_t)v;
	}
	const char *env_stop = getenv("STOP_ON_FAIL");
	int stop_on_fail = (env_stop && env_stop[0] == '1');

	for (i = 0; i < npairs; ++i)
	{
		for (c = 0; c < max_c; ++c)
		{
			int std_res = pairs[i].std_fn.int_fn((int)c);
			int ft_res = pairs[i].ft_fn.int_fn((int)c);
			ft_snprintf(namebuf, sizeof(namebuf), "%s(%d)", pairs[i].name, (int)c);

			/* quick mismatch check so we can stop early when requested */
			int mismatch = 0;
			if (pairs[i].type == TYPE_BOOL)
			{
				mismatch = (!!std_res != !!ft_res);
			}
			else /* integer comparison for conversion functions */
				mismatch = (std_res != ft_res);
			run_var_test(suite, namebuf, &ft_res, &std_res, pairs[i].type, sizeof(int));
			if (stop_on_fail && mismatch)
				return;
		}
	}
}

int main(void)
{
	t_suite *suite;

	suite = suite_create("ctype");
	test_ctype_cmp(suite);
	suite_print_summary(suite);
	/* capture results before freeing to avoid reading freed memory */
	size_t failures = suite->fails;
	suite_destroy(suite);
	return (failures == 0) ? 0 : 1;
}