/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:13:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>

int ft_add(int x, int y)
{
    int carry;
    
    while (y != 0)
    {
        carry = x & y;
        x ^= y;
        y = carry << 1;    
    }
    return (x) ;
}

//int main(int argc, char **argv)
//{
//    int n1;
//    int n2;
//    int res;
//
//    if (argc != 3)
//        return (1);
//    n1 = atoi(argv[1]);
//    n2 = atoi(argv[2]);
//    res = ft_add(n1, n2);
//    printf("%d\n", res);
//}
