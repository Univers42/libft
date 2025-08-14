#include "ft_time.h"
#include <stdint.h>

/*
 * xorshift64* variant (one 64-bit state)
 * - Uses only the provided t_rng state (no globals).
 * - Fast and allowed for school projects (not cryptographically secure).
 * - Caller must ensure rng was seeded with ft_rng_init/ft_rng_seed (non-zero).
 *
 * Reference transformation:
 *   x ^= x >> 12;
 *   x ^= x << 25;
 *   x ^= x >> 27;
 *   state = x;
 *   result = (x * 2685821657736338717ULL) & 0x7fffffffULL;
 */
int ft_rng_alea(t_rng *rng)
{
	if (!rng)
		return 0;
	uint64_t x = rng->state;
	/* xorshift64* mixing */
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	rng->state = x;
	/* multiply to improve bit diffusion, then clamp to 31 bits */
	return (int)((x * 2685821657736338717ULL) & 0x7fffffffULL);
}
