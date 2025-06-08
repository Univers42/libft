#include "ft_string.h"

static char	*ft_skip_delimiters(char *str, const char *delim)
{
	while (*str && ft_strchr(delim, *str))
		str++;
	return (str);
}

static char	*ft_find_delimiter(char *str, const char *delim)
{
	while (*str && !ft_strchr(delim, *str))
		str++;
	return (str);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last_token = NULL;
	char		*token_start;
	char		*token_end;

	if (str)
		last_token = str;
	if (!last_token || !delim)
		return (NULL);
	last_token = ft_skip_delimiters(last_token, delim);
	if (*last_token == '\0')
		return (NULL);
	token_start = last_token;
	token_end = ft_find_delimiter(last_token, delim);
	if (*token_end == '\0')
		last_token = NULL;
	else
	{
		*token_end = '\0';
		last_token = token_end + 1;
	}
	return (token_start);
}
