/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stdlib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:32:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/13 20:07:17 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDLIB_H
# define FT_STDLIB_H
# include <stdlib.h>
# include "ft_memory.h"
# include "arena.h"
# include <stdbool.h>

char	*ft_itoa(int n);
char	*ft_utoa(unsigned int n);
char	*ft_xtoa(unsigned long int num, int is_upper);
int		ft_atoi(const char *nptr);
void	ft_putnbr_base(int nbr, char *radix);
int		ft_atoi_base(const char *str, int base);
void	*ft_calloc(t_size nmemb, t_size size);
// Remove or comment out this line if you want to use your custom ft_realloc
// void    *ft_realloc(void *ptr, t_size old, t_size new);
void	skip_spaces(int *i, int size, char *str);
void	dbl_free(char **ptr);
bool	grow_buf(void **ptr, size_t *cap, size_t start);
bool	ft_realloc(void **ptr, size_t elem_size,
					size_t old_count, size_t new_count);
bool	ft_realloc_grow(void **ptr, size_t elem_size,
						size_t *cap, size_t min_needed);
bool	ft_realloc_shrink_to(void **ptr, size_t elem_size,
								size_t *cap, size_t new_cap);
                                  
#endif