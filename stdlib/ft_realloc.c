/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:06:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/05 13:13:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"
#include "ft_memory.h"
#include <stdbool.h>

bool	ft_realloc(void **ptr, size_t *cap, size_t start)
{
	size_t	new_cap;
	size_t	old_size;
	size_t	new_size;
	void	*temp;

	if (*cap == 0)
		new_cap = start;
	else
		new_cap = *cap * 2;
	
	// Determine element size based on existing pointer type
	// This is a simplified approach - ideally you'd pass element size as parameter
	if (*ptr && *cap > 0)
	{
		// Calculate element size from existing allocation
		old_size = *cap * sizeof(float); // Assume float for now
		new_size = new_cap * sizeof(float);
	}
	else
	{
		old_size = 0;
		new_size = new_cap * sizeof(float);
	}
	
	temp = malloc(new_size);
	if (temp == NULL)
		return (false);
	if (*ptr)
	{
		ft_memcpy(temp, *ptr, old_size);
		free(*ptr);
	}
	*ptr = temp;
	*cap = new_cap;
	return (true);
}