/* pgetc_eoa using singleton */
#include "input.h"

int pgetc_eoa(void)
{
    t_input *in;

    in = get_input();
    if (in->parsefile->strpush && in->parsefile->nleft == -1 && in->parsefile->strpush->ap)
        return (PEOA);
    return (pgetc());
}
