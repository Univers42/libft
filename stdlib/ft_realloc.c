/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:06:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/05 01:25:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"
#include "ft_memory.h"
#include <stdbool.h>

bool	ft_realloc(void **ptr, size_t *cap, size_t start)
{
	size_t	new_cap;
	void	*temp;

	if (*cap == 0)
		new_cap = start;
	else
		new_cap = *cap * 2;
	temp = malloc(new_cap);
	if (temp == NULL)
		return (false);
	ft_memcpy(temp, *ptr, *cap);
	free(*ptr);
	*ptr = temp;
	*cap = new_cap;
	return (true);
}