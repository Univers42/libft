#include "ft_time.h"

/* Initialize or reseed a provided RNG instance (no globals) */
void ft_rng_init(t_rng *rng, unsigned int seed)
{
	if (!rng)
		return;
	if (seed == 0u)
		seed = 1u;
	rng->state = (unsigned long long)seed;
}

void ft_rng_seed(t_rng *rng, unsigned int seed)
{
	ft_rng_init(rng, seed);
}
