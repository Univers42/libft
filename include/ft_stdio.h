/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stdio.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:30:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 18:16:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDIO_H
#define FT_STDIO_H

#include "ft_stddef.h"
#include <stddef.h>
#include "internals/output.h"
#include "internals/input.h"
#include "internals/format.h"

typedef int (*t_ops)(int, int);

char *get_next_line(int fd);
void ft_first_word(const char *str);
void ft_putchar_fd(const char c, int fd);
void ft_putstr_fd(const char *s, int fd);
void ft_putendl_fd(const char *s, int fd);
void ft_putnbr_fd(const int n, int fd);
void ft_putnbr_base(int nbr, char *radix);
int ft_printf(const char *format, ...);
int ft_dprintf(int fd, const char *format, ...);
int ft_printf(const char *format, ...);
int ft_snprintf(char *dst, size_t cap, const char *format, ...);
int ft_sprintf(char *dst, const char *format, ...);
int log_print(t_state_fd *state, const char *file, const char *format, ...);

#endif
