#include "database.h"
#include "split_csv_utils.h"
#include <stdlib.h>
#include <string.h>

char **csv_split(const char *str, char delimiter)
{
	char **result;
	size_t count;
	const char *p;
	size_t idx = 0;
	char *field;

	if (!str)
		return NULL;
	count = count_csv_fields(str, delimiter);
	result = xcalloc(count + 1, sizeof(char *));
	if (!result)
		return NULL;

	p = str;
	while (idx < count) {
		field = NULL;
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '"') {
			field = read_quoted_field(&p);
		} else {
			field = read_unquoted_field(&p, delimiter);
			if (field)
				trim_whitespace_inplace(field);
		}
		if (!field) {
			free_split(result);
			return NULL;
		}
		result[idx++] = field;
	}
	result[idx] = NULL;
	return result;
}
