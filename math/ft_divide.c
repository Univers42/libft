/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_divide.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 20:01:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ft_divide(int dividend, int divisor)
{
    if (divisor == 0)
        return 0;

    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
    unsigned int a = (dividend < 0) ? -dividend : dividend;
    unsigned int b = (divisor < 0) ? -divisor : divisor;
    unsigned int result = 0;

    for (int i = 31; i >= 0; i--)
    {
        if ((a >> i) >= b)
        {
            a = ft_subtract(a, (b << i));
            result |= (1U << i);
        }
    }
    return sign * result;
}