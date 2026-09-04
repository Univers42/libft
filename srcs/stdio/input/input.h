/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 21:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 21:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stddef.h>

/*
** get_next_line: reads one '\n'-terminated line from fd per call, keeping
** per-fd state between calls so any number of descriptors can be read in
** any interleaving. Returns a malloc'd line (with its '\n', if it had one)
** that the caller frees, or NULL at EOF, on a read error, or on allocation
** failure. Both tunables can be overridden on the command line with -D.
*/
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# ifndef GNL_FD_MAX
#  define GNL_FD_MAX 10000
# endif

char	*get_next_line(int fd);
void	get_next_line_release(int fd);

#endif
