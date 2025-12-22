#include "split_csv_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

size_t	count_csv_fields(const char *str, char delimiter)
{
	const char	*p = str;
	size_t		count;
	bool		in_quotes;

	count = 0;
	in_quotes = false;
	if (!str)
		return (0);
	while (*p)
	{
		if (*p == '"')
		{
			in_quotes = true;
			p++;
			while (*p)
			{
				if (*p == '"' && p[1] == '"')
				{
					p += 2;
					continue ;
				}
				if (*p == '"')
				{
					p++;
					in_quotes = false;
					break;
				}
				p++;
			}
		}
		else if (*p == delimiter && !in_quotes)
		{
			count++;
			p++;
		}
		else
			p++;
	}
	return (count + 1);
}



char	*trim_whitespace_inplace(char *s)
{
	char	*start;
	char	*end;
	size_t	newlen;

	if (!s)
		return (s);
	start = s;
	while (*start && (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n'))
		start++;
	if (*start == '\0')
	{
		s[0] = '\0';
		return (s);
	}
	end = start + strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
		end--;
	newlen = (size_t)(end - start + 1);
	if (start != s) memmove(s, start, newlen);
	s[newlen] = '\0';
	return (s);
}

char *trim_whitespace(char *s)
{
	return (trim_whitespace_inplace(s));
}

void free_split(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
