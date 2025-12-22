#include "database.h"

char	*read_quoted_field(const char **pp)
{
	const char	*p = *pp;
	size_t		cap;
	size_t		len;
	char		*buf;

	cap = 64;
	len = 0;
	if (!p || *p != '"')
		return (NULL);
	p++;
	buf = malloc(cap);
	if (!buf)
		return (NULL);
	while (*p)
	{
		if (*p == '"' && p[1] == '"')
		{
			if (len + 1 >= cap)
			{
				cap *= 2;
				buf = realloc(buf, cap);
				if (!buf)
					return (NULL);
			}
			buf[len++] = '"';
			p += 2;
			continue ;
		}
		if (*p == '"')
		{
			p++;
			break;
		}
		if (len + 1 >= cap)
		{
			cap *= 2;
			buf = realloc(buf, cap);
			if (!buf)
				return (NULL);
		}
		buf[len++] = *p++;
	}
	while (*p && (*p == ' ' || *p == '\t'))
		p++;
	if (*p && *p == ',')
		p++;
	buf[len] = '\0';
	*pp = p;
	return (buf);
}