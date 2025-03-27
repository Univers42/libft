/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:14:45 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/01/27 21:09:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include "TDD/test.h"
/**
 * @description:
 * Allocates (with malloc(3) and returns an array of strings obtained
 * by splitting 's' using the character 'c' as a delimiter
 * The array must end with a NULL pointer
 * @param s pointer to the first character of a string
 * @param c character delimiter is to be the reference for the split
 * @return a new multidimensional array of characters, NULL if faulty
 */

static int	safe_malloc(char **token_v, t_size pos, t_size buffer)
{
	t_size	i;

	i = 0;
	token_v[pos] = malloc(buffer);
	if (!token_v[pos])
	{
		while (i < pos)
			free(token_v[i++]);
		free(token_v);
		return (1);
	}
	return (0);
}

static int	allocate_and_copy_token(char **tokens,
									char **current_token,
									const char *start,
									t_size len)
{
	char	*token;

	if (safe_malloc(tokens, current_token - tokens, len + 1))
		return (1);
	token = *current_token;
	while (len--)
		*token++ = *start++;
	*token = '\0';
	return (0);
}

static int	allocate_and_copy_tokens(char **tokens,
									const char *s,
									char delimiter,
									t_size token_count)
{
	char		**current_token;
	const char	*start;
	t_size		len;

	current_token = tokens;
	while (*s && current_token < tokens + token_count)
	{
		while (*s == delimiter)
			s++;
		start = s;
		while (*s && *s != delimiter)
			s++;
		if (start != s)
		{
			len = s - start;
			if (allocate_and_copy_token(tokens, current_token, start, len))
				return (1);
			current_token++;
		}
	}
	return (0);
}

static int	count_segments(const char *str, char delimiter)
{
	t_size	segment;
	int		in_segment;

	segment = 0;
	in_segment = 0;
	while (*str)
	{
		if (*str == delimiter)
			in_segment = 0;
		else if (!in_segment)
		{
			in_segment = 1;
			segment++;
		}
		str++;
	}
	return (segment);
}

char	**ft_split(char const *s, char c)
{
	t_size	tokens;
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
