#include "libalias.h"

t_alias	*alias_lookup(const char *name, int check)
{
	t_alias	**app;
	t_alias	*ap;

	app = alias_find_slot(name);
	ap = *app;
	if (check && ap && (ap->flag & ALIAS_INUSE))
		return (NULL);
	return (ap);
}
