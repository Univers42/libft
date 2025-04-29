/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:04:20 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/04/29 00:08:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_string.h"
//#include "TDD/test.h"
/**
 * Concatenates the string `s2` to the end of 
 * the string `s1` and returns a new string.
 * 
 * @param s1 The first string to concatenate.
 * @param s2 The second string to concatenate to `s1`.
 * @return
 * A new string that is the result of `s1` 
 * followed by `s2`, or NULL if memory allocation fails.
 * 
 * @note 
 * The function allocates memory for 
 * the new string and copies both `s1` and `s2` into it.
 * The new string is null-terminated.
 */
char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*new_str;
	char	*dst;
	const char *src;

	if (!s1 || !s2)
		return (NULL);
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
		return (NULL);
	dst = new_str;
	src = s1;
	while (*src)
		*dst++ = *src++;
	src = s2;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (new_str);
}