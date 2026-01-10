/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:50:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 23:19:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int ft_eprintf(const char *str, ...)
{
	va_list args;
	int ret;

	va_start(args, str);
	ret = ft_veprintf(str, &args);
	va_end(args);
	return (ret);
}
