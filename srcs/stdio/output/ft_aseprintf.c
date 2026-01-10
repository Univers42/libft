/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aseprintf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:51:44 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/10 21:06:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../format/format.h"

char *ft_aseprintf(const char *str, ...)
{
    va_list args;
    char *ret;

    va_start(args, str);
    ret = ft_vaseprintf(str, &args);
    va_end(args);
    return (ret);
}
