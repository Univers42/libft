/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:07:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 20:04:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIXEL_H
# define PIXEL_H
#include <stdbool.h>
#include <stdint.h>

// Polar coordinates indices
#define LAT 0    // Latitude
#define LONG 1   // Longitude

# define ROJiO 		0xc1272d
# define VERDE		0x33cc55
# define FUCSIA 	0xff255c
# define AZUL		0x1B8EFA
# define SUPERAZUL	0x0000FF
# define CARBON		0x151515
# define WHITE		0xBBBBBB
# define DISCO		0x9A1F6A
# define BRICK_RED	0xC2294E
# define FLAMINGO	0xEC4B27
# define JAFFA		0xEF8633
# define SAFFRON	0xAAAAAA

# define DEFAULT_COLOR		JAFFA
# define BOTTOM_COLOR		AZUL
# define TOP_COLOR			BRICK_RED
# define GROUND_COLOR		SAFFRON
# define BACK_COLOR			0x151515
# define MENU_COLOR			0x202020
# define TEXT_COLOR			0xEAEAEA	
# define NUMBER_COLOR		0xF3AF3D

typedef enum e_axis
{
    X,
    Y,
    Z,
}   t_axis;

typedef struct m_colors {
	int	topcolor;
	int	groundcolor;
	int	bottomcolor;
	int	backcolor;
	int	menucolor;	
}	t_colors;

typedef struct s_color
{
    uint32_t    hex_color;
    union
    {
        uint8_t     r;
        uint8_t     g;
        uint8_t     b;
        uint8_t     a;
    }               u_raw_color;
}               t_color;

typedef struct s_point
{
    float   axis[3];
    float   polar[2];
    float   original[3];
    t_color color;
    t_axis  axes;
    bool    paint;
}           t_point;

void	translate(t_point *points, t_point move, int len);
int     valid_pixel(t_point pixel);
void	bending(t_point *points, int len, float range);
int     gradient(int startcolor, int endcolor, int len, int pix);
void	z_division(t_point *proyect, float divisor, int len);
#endif