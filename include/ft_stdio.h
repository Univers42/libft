/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stdio.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:30:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:51:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDIO_H
#define FT_STDIO_H

#include "ft_stddef.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "internals/output.h"
#include "internals/input.h"
#include "internals/format.h"

    typedef int (*t_ops)(int, int);

    char *get_next_line(int fd);
    void ft_putchar_fd(const char c, int fd);
    void ft_putstr_fd(const char *s, int fd);
    void ft_putendl_fd(const char *s, int fd);
    void ft_putnbr_fd(const int n, int fd);
    void ft_putnbr_base(int nbr, char *radix);
    int ft_printf(const char *format, ...);
    int ft_dprintf(int fd, const char *format, ...);
    int ft_eprintf(const char *format, ...);
    char *ft_aseprintf(const char *format, ...);
    int ft_snprintf(char *dst, size_t cap, const char *format, ...);
    int ft_sprintf(char *dst, const char *format, ...);
    int log_print(t_state_fd *state, const char *file, const char *format, ...);
    /* New canonical trace function name to avoid collisions with other libs. */
    void ft_trace(const char *fmt, ...);
    /* Backwards-compatibility: map old name to new implementation at compile time. */
#ifndef trace
#define trace(...) ft_trace(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif
