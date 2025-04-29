/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:09:31 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/04/28 23:32:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * write a function that converts the initial portion 
 * of the string pointed to by nptr to int representation.
 * @param char* str the string to convert
 * @param char* base the radix of the number
 * @returns int
 * @note The expected input is a string of characters 
 * that represent a number in the given base.
 * @note The expected output is the integer representation of the number.
 * @note EXAMPLE: ft_atoi("12345", "0123456789") returns 12345
 * @note EXAMPLE: ft_atoi("1010", "01") returns 10
 * @note EXAMPLE: ft_atoi("A", "0123456789ABCDEF") returns 10
 * @note EXAMPLE: ft_atoi("A", "01") returns 0
 * @note EXAMPLE: ft_atoi("A", "0123456789") returns 0
 * @note EXAMPLE: ft_atoi("A", "0123456789ABCDEF") returns 10
 * @note EXAMPLE: ft_atoi("A", "0123456789ABCDEF") returns 10
 */

int ft_atoi_base(const char *str, int base)
{
        
}

////int main()
//{
//    printf("%d\n", ft_atoi_base("101010", 2));       // Binary: 42
//    printf("%d\n", ft_atoi_base("2A", 16));         // Hexadecimal: 42
//    printf("%d\n", ft_atoi_base("--42", 10));       // Decimal: 42
//    printf("%d\n", ft_atoi_base("  +42", 10));      // Decimal: 42
//    printf("%d\n", ft_atoi_base("Z", 36));          // Base 36: 35
//    printf("%d\n", ft_atoi_base("++42", 10));       // Decimal: 42
//    printf("%d\n", ft_atoi_base("-+42", 10));       // Decimal: -42
//    printf("%d\n", ft_atoi_base("+-42", 10));       // Decimal: -42
//    printf("%d\n", ft_atoi_base("--42", 10));       // Decimal: 42
//    printf("%d\n", ft_atoi_base("0", 10));          // Decimal: 0
//    printf("%d\n", ft_atoi_base("", 10));           // Empty string: 0
//    printf("%d\n", ft_atoi_base("123abc", 10));     // Invalid characters: 123
//    printf("%d\n", ft_atoi_base("123abc", 16));     // Hexadecimal: 1194684
//    printf("%d\n", ft_atoi_base("123abc", 20));     // Base 20: 1784780
//    return 0;
//}