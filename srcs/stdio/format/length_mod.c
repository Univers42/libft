/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   length_mod.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:13:20 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 23:13:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

/* Read the length modifier that sits between the precision and the
   conversion character: %ld, %llu, %zu, %hd, %hhd.

   Without this the parser walked straight from the precision to the
   conversion, so "%ld" matched no conversion at all and was copied through
   to the output LITERALLY -- ft_snprintf(buf, n, "checked=%ld", t) wrote
   the five characters "%ld". That is a silent wrong answer rather than a
   compile error, and it has already cost a caller real time: hellish's
   update-state file came out full of format strings, and the feature that
   read it back behaved as if it had never run.

   'L' is used internally for `ll` and 'H' for `hh`, so a modifier still
   fits in the single char the spec struct carries. */
char	parse_length(const char *s, int *i)
{
	if (s[*i] == 'l' && s[*i + 1] == 'l')
		return (*i += 2, 'L');
	if (s[*i] == 'h' && s[*i + 1] == 'h')
		return (*i += 2, 'H');
	if (s[*i] == 'l' || s[*i] == 'h' || s[*i] == 'z' || s[*i] == 'j'
		|| s[*i] == 't')
		return (s[(*i)++]);
	return (0);
}

/* Pull a signed argument of the width the modifier asks for.

   h and hh read an int because that is what default argument promotion
   left in the va_list; the cast back down is what makes %hd of 70000
   print 4464, which is the whole reason someone writes %hd. */
ssize_t	fetch_signed(va_list *args, char length)
{
	if (length == 'L' || length == 'j')
		return ((ssize_t)va_arg(*args, long long));
	if (length == 'l' || length == 't')
		return ((ssize_t)va_arg(*args, long));
	if (length == 'z')
		return ((ssize_t)va_arg(*args, ssize_t));
	if (length == 'h')
		return ((ssize_t)(short)va_arg(*args, int));
	if (length == 'H')
		return ((ssize_t)(signed char)va_arg(*args, int));
	return ((ssize_t)va_arg(*args, int));
}

/* The unsigned half. Kept separate rather than casting at the call site:
   reading a variadic argument as the wrong type is undefined, not merely
   inaccurate, so the read itself has to match what was passed. */
size_t	fetch_unsigned(va_list *args, char length)
{
	if (length == 'L' || length == 'j')
		return ((size_t)va_arg(*args, unsigned long long));
	if (length == 'l' || length == 't')
		return ((size_t)va_arg(*args, unsigned long));
	if (length == 'z')
		return ((size_t)va_arg(*args, size_t));
	if (length == 'h')
		return ((size_t)(unsigned short)va_arg(*args, unsigned int));
	if (length == 'H')
		return ((size_t)(unsigned char)va_arg(*args, unsigned int));
	return ((size_t)va_arg(*args, unsigned int));
}
