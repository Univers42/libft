/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permutation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 01:16:46 by syzygy            #+#    #+#             */
/*   Updated: 2025/09/12 01:21:04 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void    swap(char *a, char *b)
{
    char temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void    permutations(char *str, int index)
{
    int i;

    if (str[index] == '\0')
    {
        printf("%s\n", str);
        return;
    }
    for (i = index; str[i] != '\0'; i++)
    {
        swap(&str[index], &str[i]);
        permutations(str, index + 1);
        swap(&str[index], &str[i]);
    }
}

int main(int argc, char **argv)
{
    char *str;

    if (argc != 2)
    {
        write(2, "Usage: ./permutation <string>\n", 30);
        return (1);
    }
    str = argv[1];
    if (!str || !*str)
    {
        write(2, "Error: Empty string\n", 20);
        return (1);
    }
    permutations(str, 0);
    return (0);
}