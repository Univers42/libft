#include "libalias.h"

static int	count_result_len(const char *s)
{
	int	len;

	len = 0;
	while (*s)
	{
		if (*s == '\'')
			len += 4;
		else
			len++;
		s++;
	}
	return (len + 2);
}

static void	fill_quoted_str(char *dest, const char *s)
{
	*dest++ = '\'';
	while (*s)
	{
		if (*s == '\'')
		{
			*dest++ = '\'';
			*dest++ = '\\';
			*dest++ = '\'';
			*dest++ = '\'';
		}
		else
			*dest++ = *s;
		s++;
	}
	*dest++ = '\'';
	*dest = '\0';
}

char	*alias_str_quote(const char *s)
{
	char	*p;
	int		len;

	if (!s)
		return (NULL);
	len = count_result_len(s);
	p = malloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	fill_quoted_str(p, s);
	return (p);
}
