/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 02:40:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 15:00:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <errno.h>
#include <string.h>

void    critical_error(const char *error)
{
    ft_eprintf("[ERROR] %s\n", error);
    exit(1);
}

void    critical_error_errno(void)
{
    ft_eprintf("[ERROR] %s\n", strerror(errno));
    exit(1);
}

void    warning_error(const char *error)
{
    ft_eprintf("[Warning] %s\n", error);
}

void    warning_error_errno(void)
{
    ft_eprintf("[WARNING] %s\n", strerror(errno));
}
