/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:00:39 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 13:29:28 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* helpers */

static int hexval(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

/* parsers - use va_list inside, signatures match jump table */

static int parse_char(t_scanf_data *data, va_list args)
{
    int c = get_char(data);
    char *dest = va_arg(args, char*);
    if (c == EOF)
        return 0;
    *dest = (char)c;
    return 1;
}

static int parse_string(t_scanf_data *data, va_list args)
{
    char *dest = va_arg(args, char*);
    int c;
    int len = 0;

    skip_whitespace(data);
    while ((c = get_char(data)) != EOF && !isspace((unsigned char)c))
    {
        dest[len++] = (char)c;
    }
    if (c != EOF)
        unget_char(data, c);
    dest[len] = '\0';
    return (len > 0) ? 1 : 0;
}

static int parse_decimal(t_scanf_data *data, va_list args)
{
    int c;
    int sign = 1;
    long num = 0;
    int digits = 0;
    int *dest = va_arg(args, int*);

    skip_whitespace(data);
    c = get_char(data);
    if (c == '+' || c == '-')
    {
        if (c == '-') sign = -1;
        c = get_char(data);
    }
    while (c != EOF && isdigit((unsigned char)c))
    {
        num = num * 10 + (c - '0');
        digits++;
        c = get_char(data);
    }
    if (c != EOF)
        unget_char(data, c);
    if (digits == 0)
        return 0;
    *dest = (int)(num * sign);
    return 1;
}

static int parse_integer(t_scanf_data *data, va_list args)
{
    int c;
    int sign = 1;
    long num = 0;
    int digits = 0;
    int base = 10;
    int *dest = va_arg(args, int*);

    skip_whitespace(data);
    c = get_char(data);
    if (c == '+' || c == '-')
    {
        if (c == '-') sign = -1;
        c = get_char(data);
    }
    if (c == '0')
    {
        int n = get_char(data);
        if (n == 'x' || n == 'X')
        {
            base = 16;
            c = get_char(data);
        }
        else
        {
            base = 8;
            if (n != EOF)
                unget_char(data, n);
            c = get_char(data);
        }
    }
    /* parse according to base */
    if (base == 10)
    {
        while (c != EOF && isdigit((unsigned char)c))
        {
            num = num * 10 + (c - '0');
            digits++;
            c = get_char(data);
        }
    }
    else if (base == 8)
    {
        while (c != EOF && (c >= '0' && c <= '7'))
        {
            num = num * 8 + (c - '0');
            digits++;
            c = get_char(data);
        }
    }
    else /* base 16 */
    {
        while (c != EOF)
        {
            int v = hexval(c);
            if (v < 0) break;
            num = num * 16 + v;
            digits++;
            c = get_char(data);
        }
    }
    if (c != EOF)
        unget_char(data, c);
    if (digits == 0)
        return 0;
    *dest = (int)(num * sign);
    return 1;
}

/* keep your jump table design: table indexed by char value */
static t_scanning *get_parser_method(void)
{
    static t_scanning table[256] = { NULL };

    /* initialize once */
    if (table['c'] == NULL && table['s'] == NULL && table['d'] == NULL && table['i'] == NULL)
    {
        table['c'] = parse_char;
        table['s'] = parse_string;
        table['d'] = parse_decimal;
        table['i'] = parse_integer;
    }
    return table;
}

/* vscanf implementation that walks format and uses the jump table */
static int vscanf_internal(const char *format, t_scanf_data *data, va_list args)
{
    int matches = 0;
    t_scanning *parsers = get_parser_method();
    int i = 0;
    int c;

    while (format[i])
    {
        if (isspace((unsigned char)format[i]))
        {
            skip_whitespace(data);
            i++;
            continue;
        }
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '%')
            {
                c = get_char(data);
                if (c == EOF || c != '%')
                {
                    if (c != EOF) unget_char(data, c);
                    break;
                }
                i++;
                continue;
            }
            /* lookup parser */
            if ((unsigned char)format[i] && parsers[(unsigned char)format[i]])
            {
                if (parsers[(unsigned char)format[i]](data, args))
                    matches++;
                else
                    break;
            }
            else
            {
                /* unsupported specifier -> stop */
                break;
            }
            i++;
        }
        else
        {
            /* literal must match exactly */
            c = get_char(data);
            if (c == EOF || c != (unsigned char)format[i])
            {
                if (c != EOF) unget_char(data, c);
                break;
            }
            i++;
        }
    }
    return matches;
}

int ft_scanf(const char *format, ...)
{
    va_list args;
    t_scanf_data data;
    int matches;

    memset(&data, 0, sizeof(data));
    data.fd = STDIN_FILENO;

    va_start(args, format);
    matches = vscanf_internal(format, &data, args);
    va_end(args);
    return matches;
}

//int main(void)
//{
//    int     i;
//    float   fp;
//    char    c;
//    char    s[81];
//
//    printf("Enter an integer, a real number, and a string: \n");
//    if (scanf("%d %f %c %s", &i, &fp, &c, s)  != 4)
//        printf("not all field were assigned");
//    else
//    {
//        printf("integer = %d\n", i);
//        printf("real number = %f\n", fp);
//        printf("character = %c\n", c);
//        printf("srtring = %s\n", s);
//    }
//}


//int main(void)
//{
//    int number;
//
//    printf("enter a hexadecimal number or anything else to quit:\n");
//    while (scanf("%x", &number))
//    {
//        printf("Hexadecimal Number = %x\n", number);
//        printf("Decimal number = %d\n", number);
//    }
//}

