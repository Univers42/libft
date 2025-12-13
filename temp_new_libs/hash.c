/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:52:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 19:53:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	djb2_hash_function(char *str)
{
	size_t	hash;
	int		c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}

t_hash	**free_hash_table(t_hash **table, size_t len, char free_data)
{
	size_t	i;
	t_hash	*next;
	t_hash	*copy;

	if (table && len > 0)
	{
		i = 0;
		while (i < len)
		{
			next = table[i];
			while (next)
			{
				if (free_data)
					ft_memdel((void**)&(next->data));
				next->hash_key = 0;
				copy = next;
				next = next->next;
				ft_memdel((void**)&(copy));
			}
			i++;
		}
		ft_memdel((void**)&table);
	}
	return (table);
}

void	*get_hash_data(t_hash **table, char *hash_str, size_t size)
{
	size_t	index;
	size_t	key;
	t_hash	*hash;

	if (!table)
		return (NULL);
	key = djb2_hash_function(hash_str);
	index = hash_index(key, size);
	hash = table[index];
	while (hash && key != hash->hash_key)
		hash = hash->next;
	if (hash)
		return (hash->data);
	return (NULL);
}

size_t	hash_index(size_t key, size_t size)
{
	return (key % size);
}

t_hash	**init_hash_table(size_t size)
{
	t_hash	**table;
	size_t	i;

	if (!(table = (t_hash**)malloc(sizeof(t_hash*) * (size + 1))))
		return (NULL);
	i = 0;
	while (i <= size)
		table[i++] = NULL;
	return (table);
}

t_hash	*init_hash(void)
{
	t_hash	*hash;

	if (!(hash = (t_hash*)malloc(sizeof(t_hash))))
		return (NULL);
	hash->data = NULL;
	hash->hash_key = 0;
	hash->next = NULL;
	return (hash);
}

t_hash	**push_hash(t_hash **table, char *hash_str, void *data, size_t size)
{
	size_t	key;
	t_hash	*hash;
	t_hash	*copy;
	size_t	index;

	if (!table || !(hash = init_hash()))
		return (NULL);
	key = djb2_hash_function(hash_str);
	index = hash_index(key, size);
	hash->data = data;
	hash->hash_key = key;
	if (table[index])
	{
		copy = table[index];
		while (copy->next)
			copy = copy->next;
		copy->next = hash;
	}
	else
		table[index] = hash;
	return (table);
}

t_hash	*set_hash(char *hash_str, char *data, size_t size)
{
	size_t	key;
	t_hash	*hash;

	if (!hash_str || !data || size == 0)
		return (NULL);
	key = djb2_hash_function(hash_str);
	if (!(hash = init_hash()))
		return (NULL);
	hash->data = data;
	hash->hash_key = key;
	return (hash);
}
