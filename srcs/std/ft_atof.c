/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 19:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_atof: strtod-style decimal parser, single pass, no allocation.
**
** Accepts what atof() accepts: leading whitespace, an optional sign, digits
** with an optional fraction, an optional exponent, and "inf" / "infinity" /
** "nan" in any case.
**
** The mantissa is accumulated as an integer in a uint64 -- 19 significant
** digits, which is more than a double can hold (~15.9) -- while a decimal
** exponent tracks the position of the point. Scaling is then done with a
** table of 10^0..10^22, every one of which is exactly representable, so a
** typical input costs exactly ONE floating-point multiply or divide after
** the integer pass. Larger exponents are applied in table-sized steps, which
** keeps the rounding error to a few ULPs instead of compounding one
** multiply per digit the way a naive "result = result * 10 + d" loop does.
*/

#include "ft_stdlib.h"
#include "ft_string.h"
#include "ft_ctype.h"

#define ATOF_MANT_DIGITS 19
#define ATOF_POW10_EXACT 22
#define ATOF_EXP_CLAMP 10000

typedef struct s_atof
{
	unsigned long long	mant;
	int					ndigits;
	int					exp10;
}	t_atof;

static const double	g_pow10[ATOF_POW10_EXACT + 1] = {
	1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11,
	1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22
};

/*
** 10^(22k) for k = 0..14, i.e. up to 10^308, each written as a literal so
** the compiler rounds it correctly once. With this, any exponent up to the
** double range costs at most TWO multiplies: one by an exact small power and
** one by a correctly-rounded big one -- instead of a chain of up to 14
** multiplies whose rounding errors compound. This is what keeps huge and
** tiny magnitudes within an ULP of a correctly-rounded strtod.
*/
#define ATOF_POW10_BIG_STEPS 14
static const double	g_pow10_big[ATOF_POW10_BIG_STEPS + 1] = {
	1e0, 1e22, 1e44, 1e66, 1e88, 1e110, 1e132, 1e154,
	1e176, 1e198, 1e220, 1e242, 1e264, 1e286, 1e308
};

/*
** Consumes a run of digits. Once the mantissa holds all the significant
** digits a double can use, further integer digits only shift the exponent
** and further fraction digits are dropped: nothing a double could have
** represented is lost. Leading zeros never count as significant.
*/
static const char	*parse_digits(const char *s, t_atof *a, int frac)
{
	while (ft_isdigit(*s))
	{
		if (a->ndigits < ATOF_MANT_DIGITS)
		{
			a->mant = a->mant * 10 + (unsigned long long)(*s - '0');
			if (a->mant != 0)
				a->ndigits++;
			if (frac)
				a->exp10--;
		}
		else if (!frac)
			a->exp10++;
		s++;
	}
	return (s);
}

static void	parse_exponent(const char *s, t_atof *a)
{
	int	sign;
	int	e;

	if (*s != 'e' && *s != 'E')
		return ;
	s++;
	sign = 1;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	if (!ft_isdigit(*s))
		return ;
	e = 0;
	while (ft_isdigit(*s))
	{
		if (e < ATOF_EXP_CLAMP)
			e = e * 10 + (*s - '0');
		s++;
	}
	a->exp10 += sign * e;
}

/*
** Applies the decimal exponent. Dividing by an exact power of ten is more
** accurate than multiplying by an inexact 1e-n, so negative exponents divide.
*/
static double	scale(double v, int exp10)
{
	int	big;
	int	neg;

	neg = (exp10 < 0);
	if (neg)
		exp10 = -exp10;
	big = exp10 / ATOF_POW10_EXACT;
	exp10 = exp10 % ATOF_POW10_EXACT;
	while (big > ATOF_POW10_BIG_STEPS)
	{
		if (neg)
			v /= g_pow10_big[ATOF_POW10_BIG_STEPS];
		else
			v *= g_pow10_big[ATOF_POW10_BIG_STEPS];
		big -= ATOF_POW10_BIG_STEPS;
	}
	if (neg)
		return (v / g_pow10[exp10] / g_pow10_big[big]);
	return (v * g_pow10[exp10] * g_pow10_big[big]);
}

double	ft_atof(const char *s)
{
	t_atof	a;
	int		neg;
	double	v;

	if (!s)
		return (0.0);
	a = (t_atof){0, 0, 0};
	while (ft_isspace(*s))
		s++;
	neg = (*s == '-');
	if (*s == '+' || *s == '-')
		s++;
	if (ft_strncasecmp(s, "nan", 3) == 0)
		return (__builtin_nan(""));
	if (ft_strncasecmp(s, "inf", 3) == 0)
	{
		if (neg)
			return (-__builtin_inf());
		return (__builtin_inf());
	}
	s = parse_digits(s, &a, 0);
	if (*s == '.')
		s = parse_digits(s + 1, &a, 1);
	parse_exponent(s, &a);
	v = scale((double)a.mant, a.exp10);
	if (neg)
		return (-v);
	return (v);
}
