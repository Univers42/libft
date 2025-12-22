/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:15:06 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:15:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

// ============= CELL FORMATTING =============

void compute_padding_if_not_too_long(char *buf, const char *text,
				size_t width, t_align align, size_t bufsize, int dlen)
{
	size_t	pad = (width > (size_t)dlen) ? (width - (size_t)dlen) : 0;
	size_t	left = 0;
	size_t	right = 0;
	size_t	out = 0;
	size_t	i;

	if (!buf || bufsize == 0)
		return;
	if (!text)
		text = "";
	buf[0] = '\0';

	if (align == ALIGN_LEFT)
	{
		ft_strlcpy(buf, text, bufsize);
		out = ft_strlen(buf);
		for (i = 0; i < pad && out + 1 < bufsize; ++i)
			buf[out++] = ' ';
		if (out < bufsize) buf[out] = '\0';
		else buf[bufsize - 1] = '\0';
		return;
	}
	else if (align == ALIGN_RIGHT)
	{
		for (i = 0; i < pad && out + 1 < bufsize; ++i)
			buf[out++] = ' ';
		if (out < bufsize)
			ft_strlcpy(buf + out, text, bufsize - out);
		else
			buf[bufsize - 1] = '\0';
		return;
	}
	else
	{
		left = pad / 2;
		right = pad - left;
		for (i = 0; i < left && out + 1 < bufsize; ++i)
			buf[out++] = ' ';
		if (out < bufsize)
			ft_strlcpy(buf + out, text, bufsize - out);
		out = ft_strlen(buf);
		for (i = 0; i < right && out + 1 < bufsize; ++i)
			buf[out++] = ' ';
		if (out < bufsize) buf[out] = '\0';
		else buf[bufsize - 1] = '\0';
		return;
	}
}

void	format_cell(char *buf, const char *text, size_t width,
			t_align align, size_t bufsize)
{
    int		dlen;
    char	tmp[MAX_LINE_LENGTH];

	if (text)
		dlen = display_width(text);
	else
		dlen = display_width("");
    if (!text)
		text = "";
    if ((size_t)dlen > width)
	{
        utf8_truncate_by_display_width(text, width, tmp, sizeof(tmp));
        ft_snprintf(buf, bufsize, "%s", tmp);
        return ;
    }
    compute_padding_if_not_too_long(buf, text, width, align, bufsize, dlen);
}

void print_rgb_color(t_color c)
{
    ft_printf("\033[38;2;%d;%d;%dm", (int)c.u_raw_color.r, (int)c.u_raw_color.g, (int)c.u_raw_color.b);
}

t_color make_color(uint8_t r, uint8_t g, uint8_t b)
{
    t_color c;

    c.z_val = 0;
    c.zf_val = 0.0f;
    c.hex_color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    c.u_raw_color.r = r;
    c.u_raw_color.g = g;
    c.u_raw_color.b = b;
    c.u_raw_color.a = 255;
    return c;
}



