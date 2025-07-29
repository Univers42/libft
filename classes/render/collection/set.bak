/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:02:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:02:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection.h"

// Quick function to add multiple shapes at once
bool collection_add_shapes(t_collection *collection, t_shape **shapes, int count) {
	if (!collection || !shapes || count <= 0)
		return false;
	
	bool success = true;
	for (int i = 0; i < count; i++) {
		if (shapes[i]) {
			if (!collection_add_shape(collection, shapes[i])) {
				success = false;
			}
		}
	}
	
	return success;
}