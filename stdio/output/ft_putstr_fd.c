/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:14:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/21 15:31:52 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Writes the string `s` to the given file descriptor `fd`.
 * 
 * @param s The string to write.
 * @param fd The file descriptor to write to.
 * @return The number of characters written.
 * 
 * @note 
 * If the string `s` is NULL, the function does nothing. 
 * The function will write the string character 
 * by character to the specified file descriptor.
 */
void	ft_putstr_fd(char *s, int fd)
{
	int	size;

	if (s)
		size = write(fd, s, ft_strlen(s));
	(void)size;
}

//int main(void)
//{
//	test_putstr_fd();
//	return 0;
//}
