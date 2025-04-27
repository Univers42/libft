/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:34:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 17:36:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"
#include <stdlib.h>

t_list	*create_linked_list(int arr[], int n)
{
	t_list	*head;
	t_list	*tail;
	t_list	*new;
	int		i;

	if (n == 0)
		return (NULL);
	head = create_node(arr[0]);
	if (!head)
		return (NULL);
	tail = head;
	i = 1;
	while (i < n)
	{
		new = create_node(arr[i]);
		if (!new)
			break ;
		tail->next = new;
		tail = new;
		i++;
	}
	return (head);
}
