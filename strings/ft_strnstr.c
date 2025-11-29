/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:44:23 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/11/29 20:10:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"

char	*ft_strnstr(const char *haystack, const char *needle, t_size n)
{
	const char	*sub_haystack;
	const char	*sub_needle;
	t_size		remaining;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack && n > 0)
	{
		sub_haystack = haystack;
		sub_needle = needle;
		remaining = n;
		while (*sub_haystack && *sub_needle
			&& *sub_haystack == *sub_needle && remaining > 0)
		{
			sub_haystack++;
			sub_needle++;
			remaining--;
		}
		if (*sub_needle == '\0')
			return ((char *)haystack);
		n--;
		haystack++;
	}
	return (NULL);
}

//int main(void)
//{
//	char *str = "hello world !";
//	char *str2 = "a new word!";
//	char *str3 = "or";
//
//	ft_printf("%s\n", ft_strnstr(str, str3, 15));	
//	ft_printf("%s\n", ft_strnstr(str2, str3, 15));	
//	ft_printf("%s\n", ft_strnstr(str, "world", 15));	
//	ft_printf("%s\n", ft_strnstr(str, "hello", 5));	
//
//	return 0;
//}
