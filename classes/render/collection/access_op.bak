/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 13:46:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/27 13:51:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection.h"

// =============================================================================
// Access operations
// =============================================================================

t_shape*	collection_get_shape(const t_collection *collection, int index)
{
	if (!collection || index < 0 || index >= collection->count)
		return (NULL);
	return collection->shapes[index];
}

int	collection_find_shape(const t_collection *collection, const t_shape *shape)
{
	int i;

	if (!collection || !shape)
		return -1;
	i = -1;
	while (++i < collection->count)
		if (collection->shapes[i] == shape)
			return (i);
	return (-1);
}

int	collection_get_count(const t_collection *collection)
{
	if (collection)
		return (collection->count);
	return (0);
}

bool	collection_is_empty(const t_collection *collection)
{
	return (collection_get_count(collection));
}

bool	collection_is_full(const t_collection *collection)
{
	return (collection_get_count(collection));
}
