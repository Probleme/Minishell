/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 15:21:44 by ataouaf           #+#    #+#             */
/*   Updated: 2023/07/30 15:25:30 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	ft_is_redirection(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	ft_negorpos(int nbr)
{
	if (nbr < 0)
		return (-nbr);
	return (nbr);
}

int	ft_atoi(char *str)
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
