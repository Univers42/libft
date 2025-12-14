/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:52:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 00:51:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_lib.h"

size_t djb2_hash_function(char *str)
{
	size_t hash;
	int c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}

t_hash_list **free_hash_table(t_hash_list **table, size_t len, char free_data)
{
	size_t i;
	t_hash_list *next;
	t_hash_list *copy;

	(void)free_data; /* parameter intentionally unused for function-pointer payloads */
	if (table && len > 0)
	{
		i = 0;
		while (i < len)
		{
			next = table[i];
			while (next)
			{
				/* function pointers don't need freeing; if free_data is set
				   the caller deliberately stored heap pointers and must free them. */
				next->hash_key = 0;
				copy = next;
				next = next->next;
				ft_memdel((void **)&(copy));
			}
			i++;
		}
		ft_memdel((void **)&table);
	}
	return (table);
}

nl_hash_fn get_hash_data(t_hash_list **table, char *hash_str, size_t size)
{
	size_t index;
	size_t key;
	t_hash_list *hash;

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

size_t hash_index(size_t key, size_t size)
{
	return (key % size);
}

t_hash_list **init_hash_table(size_t size)
{
	t_hash_list **table;
	size_t i;

	if (!(table = (t_hash_list **)malloc(sizeof(t_hash_list *) * (size + 1))))
		return (NULL);
	i = 0;
	while (i <= size)
		table[i++] = NULL;
	return (table);
}

t_hash_list *init_hash(void)
{
	t_hash_list *hash;

	if (!(hash = (t_hash_list *)malloc(sizeof(t_hash_list))))
		return (NULL);
	hash->data = NULL;
	hash->hash_key = 0;
	hash->next = NULL;
	return (hash);
}

t_hash_list **push_hash(t_hash_list **table, char *hash_str, nl_hash_fn data, size_t size)
{
	size_t key;
	t_hash_list *hash;
	t_hash_list *copy;
	size_t index;

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

t_hash_list *set_hash(char *hash_str, nl_hash_fn data, size_t size)
{
	size_t key;
	t_hash_list *hash;

	if (!hash_str || !data || size == 0)
		return (NULL);
	key = djb2_hash_function(hash_str);
	if (!(hash = init_hash()))
		return (NULL);
	hash->data = data;
	hash->hash_key = key;
	return (hash);
}
