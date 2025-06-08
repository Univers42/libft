/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:38:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/07 01:03:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include "strings/ft_string.h"
# include "memory/ft_memory.h"
# include "ctype/ft_ctype.h"
# include "lists/ft_list.h"
# include "math/ft_math.h"
# include "stdlib/ft_stdlib.h"
# include "stdio/ft_stdio.h"
# include "render/ft_render.h"
# include "debug/ft_debug.h"
# include "render/ft_colors.h"
# include "render/ft_stddef.h"
# include <unistd.h>
# include <time.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>

// String manipulation functions
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strtok(char *str, const char *delim);
char	*ft_strtrim(char const *s1, char const *set);

#endif
