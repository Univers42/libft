/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rand_extension.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:00:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 21:02:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include "xoroshiro128plusplus.h"

static inline uint64_t	rotl(const uint64_t x, int k)
{
	return ((x << k) | (x >> (64 - k)));
}

uint64_t	xoroshiro128plusplus(t_rand_state *s)
{
	const uint64_t	s0 = s->s[0];
	const uint64_t	result = rotl(s0 + s->s[1], 17) + s0;
	uint64_t		s1;

	s1 = s->s[1];
	s1 ^= s0;
	s->s[0] = rotl(s0, 49) ^ s1 ^ (s1 << 21);
	s->s[1] = rotl(s1, 28);
	return (result);
}

/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
void	xoroshiro128plusplus_jump(t_rand_state *s)
{
	uint64_t				s0;
	uint64_t				s1;
	static const uint64_t	jump[] = {0x2bd7a6a6e99c2ddc, 0x0992ccaf6a6fca05};
	int						i;
	int						b;

	s0 = 0;
	s1 = 0;
	i = -1;
	while ((size_t)++i < sizeof jump / sizeof (*jump))
	{
		b = -1;
		while (++b < 64)
		{
			if (jump[i] & UINT64_C(1) << b)
			{
				s0 ^= s->s[0];
				s1 ^= s->s[1];
			}
			xoroshiro128plusplus(s);
		}
	}
	s->s[0] = s0;
	s->s[1] = s1;
}

/* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */
void	xoroshiro128plusplus_long_jump(t_rand_state *s)
{
	static const uint64_t	long_jump[2]
		= {0x360fd5f2cf8d5d99, 0x9c6e6877736c46e3};
	uint64_t				s0;
	uint64_t				s1;
	int						i;
	int						b;

	s0 = 0;
	s1 = 0;
	i = -1;
	while ((size_t)++i < sizeof (long_jump) / sizeof (*long_jump))
	{
		b = -1;
		while (++b < 64)
		{
			if (long_jump[i] & UINT64_C(1) << b)
			{
				s0 ^= s->s[0];
				s1 ^= s->s[1];
			}
			xoroshiro128plusplus(s);
		}
	}
	s->s[0] = s0;
	s->s[1] = s1;
}
