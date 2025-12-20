/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:14:45 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/12/20 03:24:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "ft_memory.h"
#include "ft_string.h"

//#include "TDD/test.h"

int	safe_alloc(char **token_v, size_t pos, size_t buffer);
int	allocate_and_copy_token(char **tokens,
		char **current_token,
		const char *start,
		size_t len);
int	allocate_and_copy_tokens(char **tokens,
		const char *s,
		char delimiter,
		size_t token_count);
int	count_segments(const char *str, char delimiter);

/**
 * @description:
 * Allocates (with malloc(3) and returns an array of strings obtained
 * by splitting 's' using the character 'c' as a delimiter
 * The array must end with a NULL pointer
 * @param s pointer to the first character of a string
 * @param c character delimiter is to be the reference for the split
 * @return a new multidimensional array of characters, NULL if faulty
 */

#ifdef SPLIT_OPT

char	**ft_split(char const *s, char c)
{
	size_t	tokens;
	char	**token_v;

	if (!s)
		return (NULL);
	tokens = count_segments(s, c);
	token_v = malloc((tokens + 1) * sizeof(char *));
	if (!token_v)
		return (NULL);
	token_v[tokens] = NULL;
	if (allocate_and_copy_tokens(token_v, s, c, tokens))
		return (free(token_v), NULL);
	return (token_v);
}
#else

char	**ft_split(char const *str, char c)
{
	char	**out;
	int		i;
	int		start;
	int		occ;
	int		end;

	out = xcalloc((num_blocks(str, &c) + 1), sizeof(char *));
	if (out == 0)
		return (0);
	i = 0;
	occ = 0;
	while (str[i] != 0)
	{
		start = find_block(&end, str + i, &c);
		if (start == -1)
			return (out);
		out[occ] = malloc(end - start + 1);
		if (out[occ] == 0)
			return (free_list((void **)out, occ), NULL);
		ft_strlcpy(out[occ++], str + i + start, end - start + 1);
		i += end;
	}
	return (out);
}

char	**ft_split_str(char *str, char *sep)
{
	char	**out;
	int		i;
	int		start;
	int		occ;
	int		end;

	out = xcalloc((num_blocks(str, sep) + 1), sizeof(char *));
	if (out == 0)
		return (0);
	i = 0;
	occ = 0;
	while (str[i] != 0)
	{
		start = find_block(&end, str + i, sep);
		if (start == -1)
			return (out);
		out[occ] = malloc(end - start + 1);
		if (out[occ] == 0)
			return (free_list((void **)out, occ), NULL);
		ft_strlcpy(out[occ++], str + i + start, end - start + 1);
		i += end;
	}
	return (out);
}

#endif
