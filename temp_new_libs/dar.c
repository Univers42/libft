/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dar.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:27:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 19:40:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_memory.h"

void ft_putdar(char **arr)
{
	int j;

	j = -1;
	while (arr && arr[++j])
	{
		ft_putstr(arr[j]);
		ft_putchar('\n');
	}
}

void ft_putdar_fd(char **arr, int fd)
{
	int j;

	j = -1;
	while (arr && arr[++j])
	{
		ft_putstr_fd(arr[j], fd);
		ft_putchar_fd('\n', fd);
	}
}

void ft_memdel(void **ap)
{
	if (!ap)
		return;
	if (*ap != NULL)
	{
		free(*ap);
		*ap = NULL;
	}
}

char **ft_darnew(size_t size)
{
	char **arr;
	size_t i;

	arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (i <= size)
	{
		arr[i] = NULL;
		++i;
	}
	return (arr);
}

size_t ft_darlen(char **dar)
{
	size_t size;

	size = 0;
	if (!dar)
		return (size);
	while (dar[size])
		++size;
	return (size);
}

void ft_free_dar(char **arr)
{
	size_t i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		ft_memdel((void **)&arr[i]);
		++i;
	}
	free(arr);
}

char **ft_darjoin(char **first, char **second)
{
	char **res;
	size_t len1;
	size_t len2;
	size_t i;
	size_t j;

	len1 = first ? ft_darlen(first) : 0;
	len2 = second ? ft_darlen(second) : 0;
	if (len1 == 0 && len2 == 0)
		return (NULL);
	res = ft_darnew(len1 + len2);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len1)
	{
		res[i] = ft_strdup(first[j]);
		if (!res[i])
		{
			ft_free_dar(res);
			return (NULL);
		}
		++i;
		++j;
	}
	j = 0;
	while (j < len2)
	{
		res[i] = ft_strdup(second[j]);
		if (!res[i])
		{
			ft_free_dar(res);
			return (NULL);
		}
		++i;
		++j;
	}
	res[i] = NULL;
	return (res);
}

char **ft_dardup(char **arr)
{
	size_t len;
	size_t i;
	char **dst;

	if (!arr)
		return (NULL);
	len = ft_darlen(arr);
	dst = ft_darnew(len);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dst[i] = ft_strdup(arr[i]);
		if (!dst[i])
		{
			ft_free_dar(dst);
			return (NULL);
		}
		++i;
	}
	dst[i] = NULL;
	return (dst);
}

char *ft_dar2str(char **arr, char *symb)
{
	size_t i;
	size_t total;
	size_t slen;
	char *res;
	char *p;

	if (!arr)
		return (NULL);
	slen = (symb) ? ft_strlen(symb) : 0;
	total = 0;
	i = 0;
	while (arr[i])
	{
		total += ft_strlen(arr[i]);
		total += slen;
		++i;
	}
	res = ft_strnew(total);
	if (!res)
		return (NULL);
	p = res;
	i = 0;
	while (arr[i])
	{
		ft_memcpy(p, arr[i], ft_strlen(arr[i]));
		p += ft_strlen(arr[i]);
		if (slen)
		{
			ft_memcpy(p, symb, slen);
			p += slen;
		}
		++i;
	}
	return (res);
}

void ft_dar_sort(char **res)
{
	size_t i;
	char *tmp;

	if (!res || !*res)
		return;
	i = 0;
	while (res[i + 1])
	{
		if (ft_strcmp(res[i], res[i + 1]) > 0)
		{
			tmp = res[i];
			res[i] = res[i + 1];
			res[i + 1] = tmp;
			if (i != 0)
				--i;
			continue;
		}
		++i;
	}
}

char **ft_dar_add(char **dest, char **src, char **exc)
{
	size_t len_dest;
	size_t len_src;
	size_t i;
	size_t j;
	char **dar;

	if (!src || !*src)
		return (NULL);
	len_dest = dest ? ft_darlen(dest) : 0;
	len_src = ft_darlen(src);
	dar = ft_darnew(len_dest + len_src);
	if (!dar)
		return (NULL);
	i = 0;
	j = 0;
	if (dest)
	{
		while (dest[j])
		{
			dar[i] = ft_strdup(dest[j]);
			if (!dar[i])
			{
				ft_free_dar(dar);
				return (NULL);
			}
			++i;
			++j;
		}
	}
	j = 0;
	while (src[j])
	{
		if (!ft_check_exc(src[j], exc))
		{
			dar[i] = ft_strdup(src[j]);
			if (!dar[i])
			{
				ft_free_dar(dar);
				return (NULL);
			}
			++i;
		}
		++j;
	}
	dar[i] = NULL;
	return (dar);
}

int ft_check_exc(char *str, char **exc)
{
	size_t i;

	if (!str || !exc)
		return (0);
	i = 0;
	while (exc[i])
	{
		if (!ft_strcmp(exc[i], str))
			return (1);
		++i;
	}
	return (0);
}