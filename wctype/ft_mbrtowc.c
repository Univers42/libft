/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mbrtowc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:53:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 19:07:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"
#include "wchar.h"
#include "ft_memory.h"

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

size_t ft_mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
    unsigned char c;
    wchar_t wc;
    size_t needed;

    // Handle NULL input or zero-length
    if (!s || n == 0)
        return 0;

    // Initialize state if needed
    if (ps)
        ft_memset(ps, 0, sizeof(mbstate_t));

    c = (unsigned char)s[0];

    // ASCII (1-byte)
    if (c < 0x80)
    {
        if (pwc)
            *pwc = (wchar_t)c;
        return 1;
    }

    // Determine UTF-8 sequence length
    if ((c & 0xE0) == 0xC0) needed = 2;      // 110xxxxx
    else if ((c & 0xF0) == 0xE0) needed = 3; // 1110xxxx
    else if ((c & 0xF8) == 0xF0) needed = 4; // 11110xxx
    else
        return (size_t)-1; // invalid first byte

    if (needed > n)
        return (size_t)-2; // incomplete sequence

    // Validate continuation bytes
    for (size_t i = 1; i < needed; i++)
        if ((s[i] & 0xC0) != 0x80)
            return (size_t)-1; // invalid continuation

    // Assemble codepoint
    if (needed == 2)
        wc = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
    else if (needed == 3)
        wc = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
    else // needed == 4
        wc = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);

    if (pwc)
        *pwc = wc;

    return needed;
}