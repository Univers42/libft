/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 23:34:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/17 03:48:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSER_H
#define FT_PARSER_H

#include "ft_stddef.h"
#include "ds.h"

typedef struct s_parse
{
	t_state st;
	t_vec	stack;
}t_parse;

#endif
