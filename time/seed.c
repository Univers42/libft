#include "ft_time.h"
#include <stdint.h>

/*
 * ft_seed()
 * - Avoids any libc/time calls and other potentially prohibited functions.
 * - On x86/x86_64 uses RDTSC (inline asm) for run-dependent bits.
 * - On other platforms combines addresses (stack + code) and a short volatile
 *   jitter loop to produce variability between runs.
 * - Uses a small mixing routine (splitmix64-like) to produce a 32-bit seed.
 *
 * Note: Not cryptographically secure, suitable only for PRNG seeding.
 */

#if defined(__i386__) || defined(__x86_64__)
/* Read CPU timestamp counter (rdtsc) */
static inline uint64_t read_tsc(void)
{
	unsigned int hi = 0, lo = 0;
	__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)hi << 32) | (uint64_t)lo;
}
#endif

unsigned int ft_seed(void)
{
	uint64_t s = 0;

#if defined(__i386__) || defined(__x86_64__)
	/* Use TSC as primary entropy source when available */
	s = read_tsc();
#else
	/* Mix a few local addresses to get run-to-run variability (ASLR-dependent). */
	uint64_t a = (uint64_t)(uintptr_t)&s;
	uint64_t b = (uint64_t)(uintptr_t)&ft_seed;
	s = a ^ (b << 3);

	/* Short volatile jitter loop to gather more variability without syscalls. */
	{
		volatile uint64_t v = 0;
		for (unsigned int i = 0; i < 4096; ++i)
			v += ((uint64_t)(uintptr_t)&i ^ (uint64_t)i) + (v << 1);
		s ^= v;
	}
#endif

	/* Mix (splitmix64-like) to spread entropy to lower bits */
	s += 0x9e3779b97f4a7c15ULL;
	s = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ULL;
	s = (s ^ (s >> 27)) * 0x94d049bb133111ebULL;
	s = s ^ (s >> 31);

	return (unsigned int)(s & 0xffffffffu);
}

