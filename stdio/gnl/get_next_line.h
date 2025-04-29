/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:41:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 00:08:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../../ft_stddef.h"
# include "../../strings/ft_string.h"
# include "../../memory/ft_memory.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_fd_list
{
	t_size				fd;
	char				*memory;
	struct s_fd_list	*next;
}	t_fd_list;

char		*get_next_line(int fd);
char		*ft_strjoin_gnl(char *s1, const char *s2);
t_fd_list	*ft_get_fd_node(t_fd_list **fd_list, t_size fd);
void		ft_remove_fd_node(t_fd_list **fd_list, t_size fd);

#endif
