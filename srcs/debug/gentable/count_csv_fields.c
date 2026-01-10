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
