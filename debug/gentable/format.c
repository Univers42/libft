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

static void compute_padding_if_not_too_long(char *buf, const char *text,
				size_t width, t_align align, size_t bufsize, int dlen)
{
	size_t	pad;
	size_t	left;
	size_t	right;

	pad = width - dlen;
    if (align == ALIGN_LEFT)
        snprintf(buf, bufsize, "%s%*s", text, (int)pad, "");
    else if (align == ALIGN_RIGHT)
        snprintf(buf, bufsize, "%*s%s", (int)pad, "", text);
    else
	{
        left = pad / 2;
        right = pad - left;
    	ft_snprintf(buf, bufsize, "%*s%s%*s", (int)left, "", text,
			(int)right, "");
    }
}

static void	format_cell(char *buf, const char *text, size_t width,
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
    compute_padding_if_not_too_long();
}

static void print_rgb_color(t_color c)
{
    printf("\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
}

static t_color make_color(uint8_t r, uint8_t g, uint8_t b)
{
    t_color c;

	c = (t_color){r, g, b};
    return (c);
}



