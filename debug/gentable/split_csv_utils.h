#ifndef SPLIT_CSV_UTILS_H
# define SPLIT_CSV_UTILS_H

# include <stddef.h>

size_t	count_csv_fields(const char *str, char delimiter);
char	*read_quoted_field(const char **pp);
char	*read_unquoted_field(const char **pp, char delimiter);
char	*trim_whitespace_inplace(char *s);
void	free_split(char **arr);

#endif
