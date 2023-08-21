/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:43:24 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 16:26:37 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str [i] == '+')
	{
		sign = 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) +(str[i] - '0');
		i++;
	}
	return (sign * result);
}

void	put_string(int fd, char *string, int *length)
{
	if (!string)
		string = "(null)";
	while (*string)
		*length += write(fd, string++, 1);
}

void	put_digit(int fd, long long int number, int base, int *length)
{
	char	*hexadecimal;

	hexadecimal = "0123456789abcdef";
	if (number < 0)
	{
		number *= -1;
		*length += write(fd, "-", 1);
	}
	if (number >= base)
		put_digit(fd, (number / base), base, length);
	*length += write(fd, &hexadecimal[number % base], 1);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	int		length;
	va_list	args;

	length = 0;
	va_start(args, str);
	while (*str)
	{
		if ((*str == '%') && ((*(str + 1) == 's') || (*(str + 1) == 'd')
				|| (*(str + 1) == 'x')))
		{
			str++;
			if (*str == 's')
				put_string(fd, va_arg(args, char *), &length);
			else if (*str == 'd')
				put_digit(fd, (long long int)va_arg(args, int), 10, &length);
			else if (*str == 'x')
				put_digit(fd, (long long int)va_arg(args, unsigned int), 16,
					&length);
		}
		else
			length += write(fd, str, 1);
		str++;
	}
	return (va_end(args), length);
}
