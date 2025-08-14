/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:02:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 01:15:13 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "ft_scanf.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char *input;
    printf("write the input:");
    ft_scanf("%s", input);
    printf("%s\n", input);
    return (0);
}