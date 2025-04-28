/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:08:22 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/23 23:40:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_BONUS_H
# define STRUCTS_BONUS_H

# include <stdarg.h>
# include "ft_types_bonus.h"

# define HEX_LOW "0123456789abcdef"
# define HEX_UPP "0123456789ABCDEF"

/* Function pointer types for conversion and flag handling */
typedef void	(*t_flag_handler)(t_flags *, va_list);
typedef int		(*t_convert_func)(char, t_flags, va_list);

/* Structure to hold lookup tables */
typedef struct s_lookup_tables
{
	t_convert_func	convert_table[256];
	t_flag_handler	flag_table[256];
}	t_lookup_tables;

t_lookup_tables	*get_tables(void);
#endif