/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:09:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/03/24 21:30:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

int	ft_printf(const char *format, ...);
int	ft_printf_d(int n);
int	ft_printf_c(int c);
int	ft_printf_s(char *str);
int	ft_printf_percent(void);
int	ft_printf_p(void *p);
int	ft_printf_x(unsigned int n);
int	ft_printf_upper_x(unsigned int n);
int	ft_printf_u(unsigned int n);

#endif