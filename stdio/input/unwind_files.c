#include "input.h"
#include <stdlib.h>
#include <unistd.h>

/*
** unwindfiles - pop input files until 'stop' using singleton state
** Mirrors dash: popfile until parsefile == stop and basepf.prev cleared.
*/

extern t_parse_file basepf;
extern t_parse_file *parsefile;

void unwindfiles(t_parse_file *stop)
{
	t_input			*in;
	t_parse_file	*pf;

	in = get_input();
	while (in->basepf.prev || in->parsefile != stop)
		pop_file();
}
