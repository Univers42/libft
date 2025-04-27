/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hcf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:26:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int ft_hcf(int x, int y)
{
    if (y == 0)
        return x;
    return (ft_hcf(y, x % y));
}

//int main(int argc, char **argv)
//{
//    int x;
//    int y;
//
//    if (argc != 3)
//    {
//        printf("Error usage: ./usage [number1] [number2]");
//        return (1);
//    }
//    x = atoi(argv[1]);
//    y = atoi(argv[2]);
//    printf("%d\n", ft_hcf(x, y));
//    return (0);    
//}