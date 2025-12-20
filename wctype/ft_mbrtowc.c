/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mbrtowc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:53:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:11:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"
#include "wchar.h"
#include "ft_memory.h"
#include "ft_debug.h"
#include "ft_stddef.h"
#include <unistd.h>

/**
 * Converts a UTF-8 multibyte sequence into a single `wchar_t`
 * WE need to detect UTF-8 sequence length from the first byte
 * |Leading byte        |Sequence length|
 * |0xxxxxxx            |1 byte         |
 * |110xxxxx            |2 bytes		|
 * |1110xxxx			|3 bytes		|
 * |11110xxx			|4 bytes		|
 * 
 * Then we need to validate continuation bytes (10xxxxxx)
 * Aseembe codepoints:
 * 
 * @note:
 * does not fully support stateful encodings (like shift-JIS,, EUC-JP)
 * only for UTF-8;
 */

static wchar_t	asm_codepoint(size_t needed, const unsigned char *s)
{
	wchar_t		wc;

	wc = 0;
	if (needed == 2)
		wc = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
	else if (needed == 3)
		wc = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
	else if (needed == 4)
		wc = ((s[0] & 0x07) << 18)
			| ((s[1] & 0x3F) << 12)
			| ((s[2] & 0x3F) << 6)
			| (s[3] & 0x3F);
	return (wc);
}

size_t	ft_mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	unsigned char	c;
	size_t			i;
	size_t			needed;

	ft_assert(s && n);
	if (ps)
		ft_memset(ps, 0, sizeof(mbstate_t));
	c = (unsigned char)s[0];
	if (c < 0x80)
	{
		if (pwc)
			*pwc = (wchar_t)c;
		return (1);
	}
	/* determine sequence length from leading byte; if it's not a valid leading
	   byte, treat it as a single byte (avoid returning error that may loop callers) */
	if ((c & 0xE0) == 0xC0)
		needed = 2;
	else if ((c & 0xF0) == 0xE0)
		needed = 3;
	else if ((c & 0xF8) == 0xF0)
		needed = 4;
	else
	{
		/* invalid leading byte: treat as single-byte */
		if (pwc)
			*pwc = (wchar_t)c;
		return (1);
	}
	/* truncated sequence: signal incomplete so caller can provide more bytes */
	if (needed > n)
		return ((size_t)-2);
	/* validate continuation bytes; on invalid continuation, consume first byte */
	for (i = 1; i < needed; ++i)
	{
		if (((unsigned char)s[i] & 0xC0) != 0x80)
		{
			/* invalid continuation: treat first byte as single-byte */
			if (pwc)
				*pwc = (wchar_t)c;
			return (1);
		}
	}
	/* assemble codepoint */
	if (pwc)
		*pwc = asm_codepoint(needed, (const unsigned char *)s);
	return (needed);
}
