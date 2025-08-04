/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:38:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 13:38:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	Optimized gradient calculation using integer arithmetic
*	Removed caching to prevent memory corruption in multithreaded environment
*/
int	gradient(int startcolor, int endcolor, int len, int pix)
{
	int		new[3];
	int		r_start, g_start, b_start;
	int		r_end, g_end, b_end;
	
	// Bounds checking
	if (len <= 0 || pix < 0 || pix > len)
		return (startcolor);
		
	// Fast path for no gradient
	if (startcolor == endcolor)
		return (startcolor);
	
	// Extract RGB components using bit operations
	r_start = (startcolor >> 16) & 0xFF;
	g_start = (startcolor >> 8) & 0xFF;
	b_start = startcolor & 0xFF;
	
	r_end = (endcolor >> 16) & 0xFF;
	g_end = (endcolor >> 8) & 0xFF;
	b_end = endcolor & 0xFF;
	
	// Use integer math to avoid floating point operations
	new[0] = r_start + ((r_end - r_start) * pix) / len;
	new[1] = g_start + ((g_end - g_start) * pix) / len;
	new[2] = b_start + ((b_end - b_start) * pix) / len;
	
	// Clamp values to valid range
	if (new[0] > 255) new[0] = 255;
	if (new[0] < 0) new[0] = 0;
	if (new[1] > 255) new[1] = 255;
	if (new[1] < 0) new[1] = 0;
	if (new[2] > 255) new[2] = 255;
	if (new[2] < 0) new[2] = 0;
	
	return ((new[0] << 16) | (new[1] << 8) | new[2]);
}