/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctype.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:18:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/28 00:18:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CTYPE_H
# define FT_CTYPE_H

int	ft_isdigit(int c);
int	ft_isalpha(int c);
int	ft_isalnum(int c);
int	ft_isascii(int c);
int	ft_isprint(int c);
int	ft_isspace(int c);
int	ft_isupper(int c);
int	ft_islower(int c);
int	ft_toupper(int c);
int	ft_tolower(int c);
int	ft_isblank(int c);
int	ft_isxdigit(int c);
int	ft_isgraph(int c);

#endif