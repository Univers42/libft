#include "libalias.h"

t_alias	**alias_find_slot(const char *name)
{
	t_alias_state	*state;
	t_alias			**app;
	unsigned int	hashval;
	unsigned int	ch;
	const char		*p;

	state = get_alias_state();
	p = name;
	ch = (unsigned char)*p;
	hashval = ch << 4;
	while (ch)
	{
		hashval += ch;
		p++;
		ch = (unsigned char)*p;
	}
	app = &state->atab[hashval % ALIAS_TAB_SIZE];
	while (*app)
	{
		if (ft_strcmp(name, (*app)->name) == 0)
			break ;
		app = &(*app)->next;
	}
	return (app);
}
