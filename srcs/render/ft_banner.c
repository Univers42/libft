/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_banner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:05:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"
#include "ft_stdio.h"

/*
** Multi-byte border pieces: one call per edge, so a box can be drawn with
** box-drawing characters instead of ASCII. Kept separate from the char-based
** print_border because ft_print_banner's style enum is still char-based.
*/
static void	print_border_str(int width, const char *left, const char *fill,
			const char *right)
{
	int	i;

	ft_printf("%s", left);
	i = 0;
	while (i < width - 2)
	{
		ft_printf("%s", fill);
		i++;
	}
	ft_printf("%s\n", right);
}

static void	print_centered_str(const char *text, int width, const char *edge)
{
	int	text_len;
	int	padding;
	int	i;

	text_len = (int)ft_vislen(text);
	if (text_len > width - 4)
		text_len = width - 4;
	padding = (width - text_len - 2) / 2;
	ft_printf("%s", edge);
	i = -1;
	while (++i < padding)
		ft_printf(" ");
	ft_printf("%s", text);
	i = -1;
	while (++i < width - text_len - padding - 2)
		ft_printf(" ");
	ft_printf("%s\n", edge);
}

static void	print_border(int width, char corner, char line)
{
	int	i;

	ft_printf("%c", corner);
	i = 0;
	while (i < width - 2)
	{
		ft_printf("%c", line);
		i++;
	}
	ft_printf("%c\n", corner);
}

/*
** Widths are measured with ft_vislen, not ft_strlen: a coloured title costs
** the bytes of its ANSI escapes, which are zero columns wide on screen.
** Measuring them pushed the closing border out by however many bytes the
** colour took -- the same bug the table had.
**
** The line also used to stop after the text, leaving a banner whose rows had
** a left edge but no right one. It is padded out and closed properly now.
*/
static void	print_centered(const char *text, int width, char edge)
{
	int	text_len;
	int	padding;
	int	i;

	if (!text)
		return ;
	text_len = (int)ft_vislen(text);
	if (text_len > width - 4)
		text_len = width - 4;
	padding = (width - text_len - 2) / 2;
	ft_printf("%c", edge);
	i = -1;
	while (++i < padding)
		ft_printf(" ");
	ft_printf("%s", text);
	i = -1;
	while (++i < width - text_len - padding - 2)
		ft_printf(" ");
	ft_printf("%c\n", edge);
}

void	ft_print_banner(const char *title, const char *subtitle,
			t_banner_style style)
{
	int		width;
	char	corner;
	char	line;

	if (!title)
		return ;
	width = ft_get_terminal_width();
	if (width > 80)
		width = 80;
	corner = '+';
	line = '-';
	if (style == BANNER_DOUBLE)
	{
		corner = '#';
		line = '=';
	}
	print_border(width, corner, line);
	print_centered(title, width, corner);
	if (subtitle)
		print_centered(subtitle, width, corner);
	print_border(width, corner, line);
}

/*
** Box-drawing characters rather than +-|, and the same ft_vislen measurement
** as above so a coloured caption stays inside its box. The width follows the
** terminal but stops at 100 columns: a caption centred across a very wide
** terminal is harder to read, not easier.
*/
void	ft_print_boxed(const char *text)
{
	int	width;

	if (!text)
		return ;
	width = ft_get_terminal_width();
	if (width > 100)
		width = 100;
	print_border_str(width, "┌", "─", "┐");
	print_centered_str(text, width, "│");
	print_border_str(width, "└", "─", "┘");
}
