/* pgetc_eoa using singleton */
#include "input.h"

/* forward declaration to avoid implicit-int error */
int pgetc(void);

int pgetc_eoa(void)
{
    t_input *in;

    in = get_input();
    if (in->parsefile->strpush && in->parsefile->nleft == -1 && in->parsefile->strpush->ap)
        return (PEOA);
    return (pgetc());
}
