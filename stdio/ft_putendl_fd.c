/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:50:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 21:29:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Writes the string `s` followed by a newline character 
 * to the given file descriptor `fd`.
 * 
 * @param s The string to write.
 * @param fd The file descriptor to write to.
 * 
 * @return Nothing.
 * 
 * @note If the string `s` is NULL, the function does nothing. 
 * It writes the string
 * followed by a newline (`\n`) to the specified file descriptor.
 */
void	ft_putendl_fd(char *s, int fd)
{
	int	size;
	if (s)
	{
		while (*s)
			size = write(fd, s++, 1);
		size = write(fd, "\n", 1);
	}
	(void)size;
}

//int main(void)
//{
//	test_putendl_fd();
//	return 0;
//}