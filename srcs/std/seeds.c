/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seeds.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:20:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/31 13:09:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

// Linear Congruential Generator (LCG)
static void	lcg_seed(unsigned long new_seed)
{
	if (new_seed)
		get_rng_state()->seed_lcg = new_seed;
	else
		get_rng_state()->seed_lcg = 1;
}

// Xorshift RNG
static void	xorshift_seed(unsigned long new_seed)
{
	if (new_seed)
		get_rng_state()->seed_xorshift = new_seed;
	else
		get_rng_state()->seed_xorshift = 1;
}

// Middle Square RNG
static void	middle_square_seed(unsigned long new_seed)
{
	if (new_seed)
		get_rng_state()->seed_middle = new_seed;
	else
		get_rng_state()->seed_middle = 1234;
}

/*
** Selection logic.
**
** These branches used to test the enum CONSTANTS rather than the selected
** type: "if (RNG_LCG)" is "if (0)", so it was dead, and "else if
** (RNG_XORSHIFT)" is "if (1)", so it was always taken. Every ft_srand()
** therefore seeded the xorshift generator, while ft_rand() dispatched on
** state->current_type and read the LCG state -- which nothing had seeded.
** The visible symptom was that seeding did nothing at all: the same seed
** twice produced two different sequences, so no run was ever reproducible.
** The tests below now mirror ft_rand()'s dispatch exactly.
*/
void	ft_srand(unsigned int new_seed, int select)
{
	t_rng_state	*state;

	state = get_rng_state();
	if (select < 0 || select >= RNG_MAX_TYPES)
		select = RNG_LCG;
	state->current_type = (t_rng_type)select;
	if (state->current_type == RNG_XORSHIFT)
		xorshift_seed(new_seed);
	else if (state->current_type == RNG_MIDDLE_SQUARE)
		middle_square_seed(new_seed);
	else
		lcg_seed(new_seed);
}
