/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_banner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:45:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:54:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_print_banner(const char *title, const char *subtitle, t_banner_style style)
{
	int	width = 60;
	int	title_len = ft_strlen(title);
	int	subtitle_len = subtitle ? ft_strlen(subtitle) : 0;
	
	if (title_len + 4 > width)
		width = title_len + 6;
	if (subtitle_len + 4 > width)
		width = subtitle_len + 6;

	ft_printf("\n");
	
	if (style == BANNER_DOUBLE)
	{
		ft_printf("%s+", CYAN);
		ft_print_padding(width - 2, '=');
		ft_printf("+%s\n", RESET);
		
		ft_printf("%s|", CYAN);
		ft_print_centered_text(title, width - 2);
		ft_printf("|%s\n", RESET);
		
		if (subtitle)
		{
			ft_printf("%s|", CYAN);
			ft_print_centered_text(subtitle, width - 2);
			ft_printf("|%s\n", RESET);
		}
		
		ft_printf("%s+", CYAN);
		ft_print_padding(width - 2, '=');
		ft_printf("+%s\n", RESET);
	}
	ft_printf("\n");
}
