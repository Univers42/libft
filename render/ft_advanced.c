/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_advanced.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 00:41:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"
#include "ft_colors.h"

void	ft_print_notification(const char *title, const char *message,
			const char *type)
{
	const char	*color;

	if (!title || !message || !type)
		return ;
	color = GREEN_TERM;
	if (strcmp(type, "error") == 0)
		color = RED_TERM;
	else if (strcmp(type, "warning") == 0)
		color = YELLOW_TERM;
	printf("%s[%s] %s: %s%s\n", color, type, title, message, RESET_TERM);
}

void	ft_print_code_block(const char *code, const char *language)
{
	const char	*lang_name;

	if (!code)
		return ;
	lang_name = "Code";
	if (language)
		lang_name = language;
	printf("%s--- %s ---%s\n", BLUE_TERM, lang_name, RESET_TERM);
	printf("%s%s%s\n", DIM_TERM, code, RESET_TERM);
	printf("%s----------%s\n", BLUE_TERM, RESET_TERM);
}

void	ft_print_diff(const char *old_line, const char *new_line)
{
	if (!old_line || !new_line)
		return ;
	printf("%s- %s%s\n", RED_TERM, old_line, RESET_TERM);
	printf("%s+ %s%s\n", GREEN_TERM, new_line, RESET_TERM);
}

void	ft_print_emoji_status(const char *message, const char *emoji)
{
	if (!message || !emoji)
		return ;
	printf("%s %s%s %s%s\n", emoji, GREEN_TERM, message, RESET_TERM, emoji);
}
