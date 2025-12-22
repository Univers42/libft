/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 03:14:10 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 03:14:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C"
{
	#include "ft_stdio.h"
	#include <unistd.h>
	#include <string.h>
	#include <stdio.h>
	#include <fcntl.h>
}

int	main(void)
{
	char	*line;

	while ((line = get_next_line(0)))
	{
		printf("%s", line);
		free(line);
	}
	return (0);
}