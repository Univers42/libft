/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_super_trim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 22:38:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 00:14:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_stddef.h"

typedef enum e_state
{
    TRIM_SPACES = (1 << 0),
    TRIM_NUMS = (1 << 1),
    TRIM_RADIX = (1 << 2)
} t_state;

static int should_skip(char c, t_state flags)
{
    if ((flags & TRIM_SPACES) && (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
        return (1);
    if ((flags & TRIM_NUMS) && (c >= '0' && c <= '9'))
        return (1);
    if ((flags & TRIM_RADIX) && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
        return (1);
    return (0);
}

void ft_super_trim(t_addr *ptr, t_state flags)
{
    while (*(char *)*ptr && should_skip(*(char *)*ptr, flags))
        *ptr = (char *)*ptr + 1;
}

int extract_sign(t_addr *ptr)
{
    if (*(char *)*ptr == '-')
    {
        *ptr = (char *)*ptr + 1;
        return (1);
    }
    if (*(char *)*ptr == '+')
        *ptr = (char *)*ptr + 1;
    return (0);
}

int cumul_value(t_addr *ptr, t_addr target)
{
    int *val;
    int negative;

    val = (int *)target;
    negative = extract_sign(ptr);
    *val = 0;
    while (*(char *)*ptr >= '0' && *(char *)*ptr <= '9')
    {
        *val = *val * 10 + (*(char *)*ptr - '0');
        *ptr = (char *)*ptr + 1;
    }
    if (negative)
        *val = -*val;
    return (*val);
}

int extract_hex(t_addr *ptr, t_addr result)
{
    int *color = (int *)result;
    char c;
    int digit;

    *color = 0;
    if (*(char *)*ptr == '0' && (*(char *)(*ptr + 1) == 'x' || *(char *)(*ptr + 1) == 'X'))
    {
        *ptr = (char *)*ptr + 2; // Skip "0x"
        while (*(char *)*ptr)
        {
            c = *(char *)*ptr;
            if (c >= '0' && c <= '9')
                digit = c - '0';
            else if (c >= 'a' && c <= 'f')
                digit = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                digit = c - 'A' + 10;
            else
                break;
            *color = *color * 16 + digit;
            *ptr = (char *)*ptr + 1;
        }
    }
    else
    {
        #ifdef DEFAULT_WHITE
        *color = DEFAULT_WHITE;
        #else
        *color = 0xFFFFFF;  // Default to white
        #endif
    }
    return (*color);
}

void advance_ptr(t_addr *ptr, char delimiter)
{
    while (*(char *)*ptr && *(char *)*ptr != delimiter)
        *ptr = (char *)*ptr + 1;
    if (*(char *)*ptr == delimiter)
        *ptr = (char *)*ptr + 1;
}