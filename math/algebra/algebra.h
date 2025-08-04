/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algebra.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:10:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 14:22:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MATH_H
#define FT_MATH_H

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function prototypes
int     ft_add_prime_sum(int x);
int     ft_hcf(int x, int y);
int     ft_add(int x, int y);
int     ft_abs(int x);
int     ft_is_negative(int x);
int     ft_log10(int n);
int     ft_multiply(int x, int y);
int     ft_subtract(int x, int y);
int     ft_max(int a, int b);
int     ft_min(int a, int b);
int     ft_power(int base, int exp);
int     ft_sqrt(int n);
int     ft_is_prime(int x);
void    ft_fprime(int n);
int     ft_is_power2(int x);
int     ft_divide(int dividend, int divisor);
int     ft_modulo(int a, int b);
int     ft_lcm(int x, int y);
int     ft_tab_mult(int x);
double  ft_fabs(double x);
int     ft_round(double num);
float   lerp(float a, float b, float t);

#endif