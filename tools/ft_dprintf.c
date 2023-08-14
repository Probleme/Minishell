/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:43:24 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/11 13:54:15 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void    put_string(int fd, char *string, int *length)
{
    if (!string)
        string = "(null)";
    while (*string)
        *length += write(fd, string++, 1);
}

void    put_digit(int fd, long long int number, int base, int *length)
{
    char    *hexadecimal = "0123456789abcdef";

    if (number < 0)
    {
        number *= -1;
        *length += write(fd, "-", 1);
    }
    if (number >= base)
        put_digit(fd, (number / base), base, length);
    *length += write(fd, &hexadecimal[number % base], 1);
}

int    ft_dprintf(int fd, const char *str, ...)
{
    int length;

    length = 0;
    va_list    args;
    va_start(args, str);
    while (*str)
    {
        if ((*str == '%') && ((*(str + 1) == 's') || (*(str + 1) == 'd') || (*(str + 1) == 'x')))
        {
            str++;
            if (*str == 's')
                put_string(fd, va_arg(args, char *), &length);
            else if (*str == 'd')
                put_digit(fd, (long long int)va_arg(args, int), 10, &length);
            else if (*str == 'x')
                put_digit(fd, (long long int)va_arg(args, unsigned int), 16, &length);
        }
        else
            length += write(fd, str, 1);
        str++;
    }
    return (va_end(args), length);
}
