/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:45:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:56:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_print_padding(int count, char c)
{
	while (count-- > 0)
		ft_printf("%c", c);
}

void	ft_print_centered_text(const char *text, int width)
{
	int	text_len = ft_strlen(text);
	int	padding = (width - text_len) / 2;
	int	remaining = width - text_len - padding;
	
	ft_print_padding(padding, ' ');
	ft_printf("%s%s%s", BOLD, text, RESET);
	ft_print_padding(remaining, ' ');
}

void	ft_print_status(const char *message, const char *status, const char *color)
{
	ft_printf("[%s%s%s] %s\n", color, status, RESET, message);
}

void	ft_print_progress_bar(int current, int total, int width)
{
	int	filled = (current * width) / total;
	int	percentage = (current * 100) / total;
	
	ft_printf("[%s", GREEN);
	ft_print_padding(filled, '#');
	ft_printf("%s", RESET);
	ft_print_padding(width - filled, '-');
	ft_printf("] %d%%\r", percentage);
}

void	ft_clear_screen(void)
{
	ft_printf("\033[2J\033[H");
}
