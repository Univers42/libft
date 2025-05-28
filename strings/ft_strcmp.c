/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:04:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/28 10:04:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(const char *str1, const char *str2)
{
	while(*str1 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (((unsigned char) *str1) - ((unsigned char) *str2));
}