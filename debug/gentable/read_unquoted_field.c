#include "database.h"

char	*read_unquoted_field(const char **pp, char delimiter)
{
	const char	*p = *pp;
	size_t		cap = 64;
	size_t		len = 0;
	char		*buf;

	buf = malloc(cap);
	if (!buf)
		return (NULL);
	while (*p && *p != delimiter)
	{
		if (len + 1 >= cap)
		{
			cap *= 2;
			buf = realloc(buf, cap);
			if (!buf)
				return (NULL);
		}
		buf[len++] = *p++;
	}
	if (*p == delimiter)
		p++;
	buf[len] = '\0';
	*pp = p;
	return (buf);
}
