/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:03:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 19:05:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
typedef struct s_heredoc
{
	struct s_heredoc	*next;
	union s_node		*here;
	char				*eofmark;
	int					striptabs;
	
}t_heredoc;
#endif