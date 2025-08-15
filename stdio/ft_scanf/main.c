/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:02:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 13:02:44 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "ft_scanf.h"
#include <unistd.h>
#include "libft.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char    input[256];
    int     i;

    ft_printf("write the input:");
    if (ft_scanf("%s %d", input, &i) != 2)
        return (write(1, "error", 5), 1);
    ft_printf("%s\n", input);
    ft_printf("\ninteger: %i\n", i);
    return (0);
}