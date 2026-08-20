/* ************************************************************************** */
/*   test_printf_length.c — %ld %lu %lld %llu %zu %hd %hhd %lx and friends.   */
/*                                                                            */
/*   The parser had no length-modifier step at all, so "%ld" matched no       */
/*   conversion and was copied through LITERALLY: ft_snprintf(b, n, "%ld", t) */
/*   wrote the three characters "%ld". A silent wrong answer, not a compile   */
/*   error -- which is how it survived: a caller storing a timestamp got a    */
/*   file full of format strings and a feature that behaved as if it had      */
/*   never run.                                                               */
/*                                                                            */
/*   Widening the fetch then exposed a second layer: writer_signed and the    */
/*   hex writers recursed through ft_abs(), declared `int ft_abs(int)`, so    */
/*   every value past 32 bits was truncated on the way out.                   */
/* ************************************************************************** */

#include "libft_test.h"
#include "ft_stdio.h"
#include <limits.h>
#include <string.h>
#include <sys/types.h>

/* Every case here is what the C library prints for the same format and
   value; they were diffed against glibc rather than written from memory. */
static void	test_long(void)
{
	char	b[128];

	ft_snprintf(b, sizeof(b), "%ld", (long)1787179717L);
	t_check_str("ld_epoch", b, "1787179717");
	ft_snprintf(b, sizeof(b), "%ld", (long)-42L);
	t_check_str("ld_negative", b, "-42");
	ft_snprintf(b, sizeof(b), "%lu", (unsigned long)4294967296UL);
	t_check_str("lu_past_32_bits", b, "4294967296");
	ft_snprintf(b, sizeof(b), "%ld", (long)LONG_MAX);
	t_check_str("ld_long_max", b, "9223372036854775807");
	ft_snprintf(b, sizeof(b), "%ld", (long)LONG_MIN);
	t_check_str("ld_long_min", b, "-9223372036854775808");
}

static void	test_long_long(void)
{
	char	b[128];

	ft_snprintf(b, sizeof(b), "%lld", (long long)-9223372036854775807LL);
	t_check_str("lld_huge_negative", b, "-9223372036854775807");
	ft_snprintf(b, sizeof(b), "%llu", (unsigned long long)18446744073709551615ULL);
	t_check_str("llu_ullong_max", b, "18446744073709551615");
	ft_snprintf(b, sizeof(b), "%llx", (unsigned long long)0xdeadbeefcafeULL);
	t_check_str("llx_past_32_bits", b, "deadbeefcafe");
}

static void	test_size_and_short(void)
{
	char	b[128];

	ft_snprintf(b, sizeof(b), "%zu", (size_t)123456789);
	t_check_str("zu", b, "123456789");
	ft_snprintf(b, sizeof(b), "%zd", (ssize_t)-99);
	t_check_str("zd_negative", b, "-99");
	ft_snprintf(b, sizeof(b), "%hd", 70000);
	t_check_str("hd_truncates_to_short", b, "4464");
	ft_snprintf(b, sizeof(b), "%hhd", 300);
	t_check_str("hhd_truncates_to_char", b, "44");
}

static void	test_hex_and_flags(void)
{
	char	b[128];

	ft_snprintf(b, sizeof(b), "%lx", (unsigned long)0xdeadbeefcafeUL);
	t_check_str("lx", b, "deadbeefcafe");
	ft_snprintf(b, sizeof(b), "%lX", (unsigned long)0xdeadbeefcafeUL);
	t_check_str("lX", b, "DEADBEEFCAFE");
	ft_snprintf(b, sizeof(b), "%5ld", (long)7L);
	t_check_str("width_with_length", b, "    7");
	ft_snprintf(b, sizeof(b), "%-8ld|", (long)7L);
	t_check_str("left_justify_with_length", b, "7       |");
	ft_snprintf(b, sizeof(b), "%08ld", (long)7L);
	t_check_str("zero_pad_with_length", b, "00000007");
	ft_snprintf(b, sizeof(b), "%+ld", (long)7L);
	t_check_str("plus_with_length", b, "+7");
}

/* The plain conversions have to keep behaving exactly as before: widening
   the writers is only safe if the 32-bit paths are untouched. */
static void	test_no_regression(void)
{
	char	b[128];

	ft_snprintf(b, sizeof(b), "%d", 42);
	t_check_str("plain_d", b, "42");
	ft_snprintf(b, sizeof(b), "%d", INT_MIN);
	t_check_str("plain_d_int_min", b, "-2147483648");
	ft_snprintf(b, sizeof(b), "%u", 42u);
	t_check_str("plain_u", b, "42");
	ft_snprintf(b, sizeof(b), "%u", 4294967295u);
	t_check_str("plain_u_uint_max", b, "4294967295");
	ft_snprintf(b, sizeof(b), "%x", 255u);
	t_check_str("plain_x", b, "ff");
	ft_snprintf(b, sizeof(b), "%X", 255u);
	t_check_str("plain_X", b, "FF");
	ft_snprintf(b, sizeof(b), "[%5d][%-5d][%05d]", 42, 42, 42);
	t_check_str("plain_flags", b, "[   42][42   ][00042]");
	ft_snprintf(b, sizeof(b), "%s/%c/%%", "abc", 'z');
	t_check_str("plain_str_char_pct", b, "abc/z/%");
}

int	main(void)
{
	t_begin("printf_length");
	test_long();
	test_long_long();
	test_size_and_short();
	test_hex_and_flags();
	test_no_regression();
	return (t_end());
}
