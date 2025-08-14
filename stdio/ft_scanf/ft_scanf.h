/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:56:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 01:13:32 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCANF_H
# define FT_SCANF_H
#include <stdarg.h>
#include <stdio.h>

int vscanf(const char *fmt, va_list ap);
int scanf(const char *fmt, ...);
int fscanf(FILE *stream, const char *fmt, ...);
int ft_scanf(const char *fmt, ...);
#endif