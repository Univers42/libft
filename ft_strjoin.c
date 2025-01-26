/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:04:20 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/01/26 21:02:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (!s1 || !s2)
		return (NULL);
	res = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (res == NULL)
		return (NULL);
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[k])
	{
		res[i + k] = s2[k];
		k++;
	}
	res[i + k] = '\0';
	return (res);
}
