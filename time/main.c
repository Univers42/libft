/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:41:40 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 00:43:38 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_time.h"

int main(void)
{
	unsigned int seed = ft_seed();
	t_rng rng;

	ft_rng_init(&rng, seed);
	printf("ft_rng_init(%u)\n", seed);
	printf("FT_RAND_MAX = %d\n", FT_RAND_MAX);

	/* print a few random numbers from the local RNG */
	for (int i = 0; i < 10; ++i)
		printf("ft_rng_rand() -> %d\n", ft_rng_rand(&rng));

	return 0;
}
}
