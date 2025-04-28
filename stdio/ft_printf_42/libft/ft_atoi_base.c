/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:09:31 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/23 14:22:09 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
//static int	nbr_inbase(char c, int base)
//{
//	if (base <= 10)
//		return (c >= '0' && c <= '9');
//	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= ('A' + base - 10)) ||
//	(c >= 'a' && c <= ('a' + base - 10)));
//} 
//
//int	ft_atoi_base(const char *str, int base)
//{
//	int		i;
//	int		nbr;
//	int		sign;
//
//	i = 0;
//	if (!str[0] || (base < 2 || base > 16))
//		return (0);
//	nbr = 0;
//	sign = 1;
//	while (str[i] == '\t' || str[i] == '\v' || str[i] == '\n' ||
//		str[i] == ' ' || str[i] == '\r' || str[i] == '\f')
//		i += 1;
//	if (str[i] == '-' || str[i] == '+')
//		if (str[i++] == '-')
//			sign *= -1;
//	while (str[i] && nbr_inbase(str[i], base))
//	{
//		if (str[i] >= 'A' && 'F' >= str[i])
//			nbr = (nbr * base) + (str[i] - 'A' + 10);
//		else if (str[i] >= 'a' && 'f' >= str[i])
//			nbr = (nbr * base) + (str[i] - 'a' + 10);
//		else
//			nbr = (nbr * base) + (str[i] - '0');
//		i += 1;
//	}
//	return (nbr * sign);
//}
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