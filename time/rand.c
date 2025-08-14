#include "ft_time.h"

/* forward declaration of the new algorithm implemented in alea.c */
int ft_rng_alea(t_rng *rng);

/* public API — reuse the existing name but delegate to xorshift64* */
int ft_rng_rand(t_rng *rng)
{
	return ft_rng_alea(rng);
}
	if (!rng)
		return 0;
	rng->state = (1103515245ULL * rng->state + 12345ULL) & 0x7fffffffULL;
	return (int)rng->state;
}
