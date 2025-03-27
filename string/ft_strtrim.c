/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 14:19:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 21:22:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * @param s1 the string that has to be shortened
 * @param set The characters to eliminate the string.
 * @return the string shortened or NULL is issue with memoria
 */
//char	*ft_ft_strtrim(const char *s1, const char *set)
//{
//	char	*trimmed_str;
//	t_size	start;
//	t_size	end;
//	t_size	len;
//
//	if (!s1 && !set)
//		return (NULL);
//	start = 0;
//	while (s1[start] && ft_strchr(set, s1[start]))
//		start++;
//	end = ft_strlen(s1);
//	while (end > start && ft_strchr(set, s1[end - 1]))
//		end--;
//	len = end-start;
//	trimmed_str = malloc(len + 1);
//	if (!trimmed_str)
//		return (NULL);
//	ft_strlcpy(trimmed_str, s1 + start, len + 1);
//	return (trimmed_str);
//}
/**RETRY : first attempt */
/**
 *
 * @param s1 first string
 * @param set set of rules we want to trim for the string
 * @return a new pointer to first byte with some length
 * @note
 * The strtrim work this following way : 
 * first we need to knwo how large is the whitespace 
 * from the start of thes string up to the first character that doesn't match 
 * the set. Next we we proceed by reversin 
 * the index starting from the length and decrementing
 * the index up to the first character that doesn't match with the string
 * once we get the start index and the end index, 
 * by a simple calcul, we can retrieve the length
 * Finally the substring of this computing function will 
 * be hold by a new string that will have
 * exactly the good amount of length.
 */

/**
 * Trims the characters from both ends of 
 * the string `s1` that belong to 
 * the set of characters specified in `set`.
 * 
 * @param s1 The string to be trimmed.
 * @param set The set of characters to be trimmed from both ends of `s1`.
 * @return 
 * A new string that is a trimmed version of `s1`,
 * or NULL if memory allocation fails.
 * 
 * @note 
 * The function calculates the length of the leading and 
 * trailing characters that match with `set`.
 * It then creates a new string that contains the substring 
 *from `s1`, excluding the matching characters.
 * If `set` is NULL or `s1` is NULL, the behavior is undefined. 
 * If no characters need to be trimmed, 
 *       a copy of `s1` will be returned.
 */
char	*ft_strtrim(const char *s1, const char *set)
{
	char	*new_string;
	t_size	start;
	t_size	end;
	t_size	new_len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strrchr(set, s1[end - 1]))
		end--;
	new_len = end - start;
	new_string = malloc(new_len +1);
	if (!new_string)
		return (NULL);
	ft_strlcpy(new_string, s1 + start, new_len + 1);
	return (new_string);
}
// TODO test
//int main()
//{
//	char *s1 = "   hello world!   ";
//	char *set = " ";
//	char *trimmed;
//	
//	trimmed = ft_strtrim(s1, set);
//	if (trimmed)
//	{
//		printf("old string : '%s'\n", s1);
//		printf("new_string : '%s'\n", trimmed);
//		free(trimmed);
//		return 0;
//	}
//	return 1;
//}