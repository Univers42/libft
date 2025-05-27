/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_advanced.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 19:02:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_print_notification(const char *title, const char *message, const char *type)
{
	const char *color = GREEN;
	const char *icon = "[INFO]";
	
	if (ft_strncmp(type, "error", 5) == 0)
	{
		color = RED;
		icon = "[ERROR]";
	}
	else if (ft_strncmp(type, "warning", 7) == 0)
	{
		color = YELLOW;
		icon = "[WARN]";
	}
	else if (ft_strncmp(type, "success", 7) == 0)
	{
		color = GREEN;
		icon = "[SUCCESS]";
	}
	
	ft_printf("\n%s╭─ %s %s%s\n", color, icon, title, RESET);
	ft_printf("%s│ %s%s\n", color, message, RESET);
	ft_printf("%s╰─────────────────────────────%s\n\n", color, RESET);
}

void	ft_print_emoji_status(const char *message, const char *emoji)
{
	ft_printf("%s %s %s\n", emoji, message, emoji);
}

void	ft_print_code_block(const char *code, const char *language)
{
	int width = ft_strlen(code) + 4;
	if (width < 20)
		width = 20;
		
	ft_printf("%s+- %s -", CYAN, language);
	ft_print_padding(width - ft_strlen(language) - 4, '-');
	ft_printf("+%s\n", RESET);
	
	ft_printf("%s| %s%s", CYAN, RESET, code);
	ft_print_padding(width - ft_strlen(code) - 2, ' ');
	ft_printf("%s|%s\n", CYAN, RESET);
	
	ft_printf("%s+", CYAN);
	ft_print_padding(width, '-');
	ft_printf("+%s\n", RESET);
}

void	ft_print_diff(const char *old_line, const char *new_line)
{
	ft_printf("%s- %s%s\n", RED, old_line, RESET);
	ft_printf("%s+ %s%s\n", GREEN, new_line, RESET);
}

void	ft_typewriter_effect(const char *text, int delay_ms)
{
	while (*text)
	{
		ft_printf("%c", *text++);
		usleep(delay_ms * 1000);
	}
	ft_printf("\n");
}
