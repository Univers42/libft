#include "libalias.h"

int	alias_unset(const char *name)
{
	t_alias	**app;

	app = alias_find_slot(name);
	if (*app)
	{
		*app = alias_free(*app);
		return (0);
	}
	return (1);
}
