/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xoroshiro123plusplus.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:02:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 21:02:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XOROSHIRO128PLUSPLUS_H
# define XOROSHIRO128PLUSPLUS_H
# include <stdint.h>

typedef struct s_rand_state
{
	uint64_t	s[2];
}	t_rand_state;

uint64_t	xoroshiro128plusplus(t_rand_state *s);

/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
void		xoroshiro128plusplus_jump(t_rand_state *s);

#endif
