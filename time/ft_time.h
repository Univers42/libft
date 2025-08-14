/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:40:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 00:43:49 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TIME_H
# define FT_TIME_H

# include <stdint.h>

/* Maximum value returned by ft_rng_rand() */
# ifndef FT_RAND_MAX
#  define FT_RAND_MAX 2147483647 /* 2^31 - 1 */
# endif

/* RNG type (user-visible structure; can be stack-allocated) */
typedef struct s_ft_rng
{
	unsigned long long	state;
}						t_rng;

/* Initialize/seed an RNG instance (seed 0 -> internally converted to 1) */
void	ft_rng_init(t_rng *rng, unsigned int seed);
void	ft_rng_seed(t_rng *rng, unsigned int seed);

/* Generate a pseudo-random integer in [0, FT_RAND_MAX] using the given RNG */
int		ft_rng_rand(t_rng *rng);

/* Provide a portable program-derived seed (no time(NULL)) */
unsigned int	ft_seed(void);

#endif
