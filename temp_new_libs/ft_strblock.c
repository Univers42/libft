/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strblock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:49:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:08:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_lib.h"

void ft_str_clear(char *s)
{
	while (s && *s)
		*s = '\0';
}

void	ft_putnbr(int nb)
{
	ft_putnbr_fd(nb, STDOUT_FILENO);
}

void ft_putstr(const char *str)
{
	ft_putstr_fd(str, STDOUT_FILENO);
}

void ft_putchar(const int c)
{
	ft_putchar_fd(c, STDOUT_FILENO);
}

char *ft_strcat(const char *src1, const char *src2)
{
	char *buffer;
	size_t len1;
	size_t len2;

	if (!src1 || !src2)
		return (NULL);
	len1 = ft_strlen(src1);
	len2 = ft_strlen(src2);
	buffer = ft_strnew(len1 + len2);
	if (!buffer)
		return (NULL);
	ft_memcpy(buffer, src1, len1);
	ft_memcpy(buffer + len1, src2, len2);
	buffer[len1 + len2] = '\0';
	return (buffer);
}

void ft_strdel(char **da)
{
	if (da)
	{
		free(*da);
		*da = NULL;
	}
}

int ft_strequ(char const *s1, char const *s2)
{
	if (!s1 && !s2)
		return (1);
	if (s1 && s2)
	{
		while (*s1 != '\0' || *s2 != '\0')
		{
			if (*s1 != *s2)
				return (0);
			s1++;
			s2++;
		}
	}
	return (1);
}

int ft_strnequ(char const *s1, char const *s2, size_t n)
{
	unsigned int i;

	i = n;
	if ((!s1 && !s2) || !n)
		return (1);
	if (s1 && s2 && n)
	{
		while (i != 0)
		{
			if (*s1 != *s2)
				return (0);
			s1++;
			s2++;
			i--;
		}
	}
	return (1);
}

char *ft_strnew(size_t size)
{
	char *arr;

	if (size >= 9223372036854775807)
		return (NULL);
	arr = (char *)malloc(sizeof(char) * (size + 1));
	if (!arr)
		return (NULL);
	arr[size] = '\0';
	while (size--)
		arr[size] = '\0';
	return (arr);
}

char *ft_del_sym_str(char *str, char *delsym)
{
	char *buffer;
	char *newstr;
	int i;
	int j;

	if (!str || !delsym)
		return (NULL);
	if (!(buffer = ft_strnew(ft_strlen(str))))
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && ft_is_str_contain(delsym, str[i]))
		i++;
	while (str[i])
	{
		if (!ft_is_str_contain(delsym, str[i]))
			buffer[j++] = str[i];
		i++;
	}
	buffer[j] = '\0';
	newstr = ft_strdup(buffer);
	ft_memdel((void **)&buffer);
	return (newstr);
}

size_t ft_get_cnt_words(const char *str, char *spr)
{
	size_t i;
	size_t count;
	short in_word;

	count = 0;
	if (!str || !spr)
		return (count);
	i = -1;
	in_word = 0;
	while (str[++i])
	{
		if (!ft_is_str_contain(spr, str[i]) && !in_word)
		{
			in_word = !in_word;
			count++;
		}
		if (ft_is_str_contain(spr, str[i]) && in_word)
			in_word = !in_word;
	}
	return (count);
}

int ft_is_str_contain(char *str, char sym)
{
	size_t i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] == sym)
			return (1);
	return (0);
}

char *ft_last_word_pos(const char *str, char *seps)
{
	size_t len;
	short in_word;

	if (!str || !seps)
		return (NULL);
	if (!(len = ft_strlen(str)))
		return (NULL);
	in_word = 0;
	while ((int)--len != -1)
	{
		if (ft_is_str_contain(seps, str[len]) && in_word)
			return ((char *)str + (len + 1));
		if (!ft_is_str_contain(seps, str[len]) && !in_word)
			in_word = !in_word;
	}
	return ((in_word) ? (char *)str : NULL);
}

int ft_match(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	else if (!*s1 && !*s2)
		return (1);
	else if (*s1 != '\0' && *s2 == '*')
		return (ft_match(s1 + 1, s2) || ft_match(s1, s2 + 1));
	else if (*s1 == '\0' && *s2 == '*')
		return (ft_match(s1, s2 + 1));
	else if (*s1 == *s2)
		return (ft_match(s1 + 1, s2 + 1));
	else
		return (0);
}

size_t ft_num_match(char **str, char *arg)
{
	int i;
	size_t count;

	i = -1;
	count = 0;
	if (!str || !arg)
		return (count);
	while (str[++i])
		if (ft_match(str[i], arg))
			count++;
	return (count);
}

char *ft_stradd(char *buf, char *s, size_t i)
{
	char *str;

	if (!s || !buf)
		return (buf);
	if (i >= ft_strlen(buf))
		return (ft_strcat(buf, s));
	if (!(str = ft_strdup(buf + i)))
		return (NULL);
	ft_strcpy(buf + i, s);
	buf = ft_strcat(buf, str);
	ft_strdel(&str);
	return (buf);
}

char *ft_strdel_el(char *buf, int i)
{
	if (i < 0)
		return (buf);
	if (i >= (int)ft_strlen(buf))
		return (buf);
	ft_strcpy(buf + i, buf + i + 1);
	return (buf);
}

char *ft_strdup_realloc(char *str, short len)
{
	char *arr;

	arr = str;
	if (!(str = ft_strnew(len)))
		return (NULL);
	if (arr)
		str = ft_strcat(str, arr);
	ft_memdel((void **)&arr);
	return (str);
}

char *ft_strncat(char *dest, const char *src, size_t n)
{
	size_t dlen;
	size_t i;

	if (!dest || !src)
		return (dest);
	dlen = ft_strlen(dest);
	i = 0;
	while (i < n && src[i])
	{
		dest[dlen + i] = src[i];
		++i;
	}
	dest[dlen + i] = '\0';
	return (dest);
}
