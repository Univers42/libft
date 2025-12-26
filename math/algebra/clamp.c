/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clamp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:23:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 21:09:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algebra.h"

//#define CLAMP(x, min, max) \
//    _Generic((x), \
//        int: iclamp, \
//        float: fclamp, \
//        double: dclamp \
//    )(x, min, max)
//

int iclamp(int x, int min, int max)
{
    if (x < min)
        return min;
    else if (x > max)
        return (max);
    else
        return (x);
}

float fclamp(float x, float min, float max)
{
    if (x < min)
        return (min);
    else if (x > max)
        return (max);
    else
        return (x);
}

double dclamp(double x, double min, double max)
{
    if (x < min)
        return (min);
    else if (x > max)
        return (max);
    else
        return (x);
}
