/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:56:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 13:28:18 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCANF_H
# define FT_SCANF_H
#include <stdarg.h>
#include <stdio.h>

# define BUFFER_SIZE 1024

typedef struct s_scanf_data
{
    char    buffer[BUFFER_SIZE];
    int     buf_pos;
    int     buf_len;
    int     fd;
} t_scanf_data;

/* parser function type - keeps your jump table design */
typedef int (*t_scanning)(t_scanf_data *data, va_list args);

int     vscanf(const char *fmt, va_list ap);
int     scanf(const char *fmt, ...);
int     fscanf(FILE *stream, const char *fmt, ...);
int     ft_scanf(const char *fmt, ...);
void    skip_whitespace(t_scanf_data *data);
void    unget_char(t_scanf_data *data, int c);
int     get_char(t_scanf_data *data);

#endif