/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:53:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:23:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DEBUG_H
# define FT_DEBUG_H
# include "../render/ft_stddef.h"

void	ft_print_ascii(unsigned char *p, unsigned int size);
void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr);
void	*ft_print_memory(void *addr, unsigned int size);
void	*ft_print_array(int *arr, int size, char mode);
void    print_array(int *arr, int size);
#endif