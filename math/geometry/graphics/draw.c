/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:11:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 21:15:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <threads.h>
#include <unistd.h>
#include "ft_math.h"

/***
 * extract each channel for te pixel cellular
 */
void	draw_pixel(uint8_t *pixel, uint32_t color)
{
    *(pixel++) = xtr_a(color);	//alpha channel
    *(pixel++) = xtr_r(color);	//red channel
    *(pixel++) = xtr_g(color);
    *(pixel++) = xtr_b(color);
}
