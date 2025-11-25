#include "libalias.h"

t_alias	*alias_free(t_alias *ap)
{
	t_alias	*next;

	if (ap->flag & ALIAS_INUSE)
	{
		ap->flag |= ALIAS_DEAD;
		return (ap);
	}
	next = ap->next;
	free(ap->name);
	free(ap->val);
	free(ap);
	return (next);
}
