/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyl-syzygy <dyl-syzygy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:51:16 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/11 15:52:46 by dyl-syzygy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Duplicates a string up to n characters.
 *
 * This function creates a new string by duplicating the input string 's'
 * up to a maximum of 'n' characters. The resulting string will always be
 * null-terminated. If the length of 's' is less than 'n', only the actual
 * length of the string will be duplicated.
 *
 * ⚠️ WARNING: This function uses malloc and requires the caller to free
 * the returned pointer when it's no longer needed to avoid memory leaks.
 *
 * @param s The source string to duplicate
 * @param n The maximum number of characters to duplicate
 * @return A pointer to the newly allocated duplicated string,
 *         or NULL if allocation fails
 */
char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	if (len > n)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}