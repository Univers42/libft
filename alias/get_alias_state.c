#include "libalias.h"

t_alias_state	*get_alias_state(void)
{
	static t_alias_state	instance;
	static int				is_init;

	if (!is_init)
	{
		ft_bzero(&instance, sizeof(t_alias_state));
		is_init = 1;
	}
	return (&instance);
}
