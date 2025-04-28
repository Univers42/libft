/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:41:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/28 00:44:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_size	ft_strclen(const char *s, int c)
{
	t_size	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	*ft_strndup(const char *s, t_size n)
{
	char	*dup;
	char	*d;

	dup = (char *)malloc(n + 1);
	if (!dup)
		return (NULL);
	d = dup;
	while (*s && n--)
		*d++ = *s++;
	*d = '\0';
	return (dup);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	char		*new_str;
	char		*ptr;
	t_size		len2;
	t_size		len1;

	len1 = ft_strclen(s1, '\0');
	len2 = ft_strclen(s2, '\0');
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	ptr = new_str;
	while (s1 && *s1)
		*ptr++ = *s1++;
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (free(s1 - len1), new_str);
}

t_fd_list	*ft_get_fd_node(t_fd_list **fd_list, t_size fd)
{
	t_fd_list	*current;
	t_fd_list	*new_node;

	current = *fd_list;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	new_node = malloc(sizeof(t_fd_list));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->memory = NULL;
	new_node->next = *fd_list;
	*fd_list = new_node;
	return (new_node);
}

void	ft_remove_fd_node(t_fd_list **fd_list, t_size fd)
{
	t_fd_list	*prev;
	t_fd_list	*current;

	prev = NULL;
	current = *fd_list;
	while (current)
	{
		if (current->fd == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				*fd_list = current->next;
			(free(current->memory), free(current));
			return ;
		}
		prev = current;
		current = current->next;
	}
}
