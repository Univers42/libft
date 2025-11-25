#include "libalias.h"

static int	is_flag_a(const char *arg)
{
	return (arg[0] == '-' && arg[1] == 'a' && arg[2] == '\0');
}

int	unalias_cmd(int argc, char **argv)
{
	int	i;
	int	ret;

	i = 1;
	while (i < argc && is_flag_a(argv[i]))
	{
		alias_remove_all();
		return (0);
	}
	ret = 0;
	while (i < argc)
	{
		if (alias_unset(argv[i]))
		{
			ft_putstr_fd("unalias: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": not found\n", 2);
			ret = 1;
		}
		i++;
	}
	return (ret);
}
