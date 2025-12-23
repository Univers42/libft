#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char const *argv[])
{
	int	pid;

	(void)argc;
	(void)argv;
	pid = fork();
	open("infile", O_RDONLY);
	while (1)
	{
		printf("Helloo miniHELL %i\n", pid);
		sleep(1);
	}
	return (0);
}
