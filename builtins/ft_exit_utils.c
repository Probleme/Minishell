/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:09:36 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:33:31 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int64_t	ft_erratoll(const char *str, int *err)
{
	int64_t	nb;
	int		sig;

	sig = 1;
	nb = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sig *= -1;
		str++;
	}
	*err = 0;
	while (*str >= '0' && *str <= '9' && *err == 0)
	{
		nb = nb * 10 + (*str++ - '0') * sig;
		if (nb < 0 && sig == 1)
			*err = 1;
		else if (nb > 0 && sig == -1)
			*err = 2;
	}
	return (nb);
}

int64_t	ft_atoll(const char *str)
{
	int64_t	nb;
	int		sig;

	sig = 1;
	nb = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sig *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nb = nb * 10 + (*str++ - '0') * sig;
		if (nb > LONG_MAX)
			return (-1);
		else if (nb < LONG_MIN)
			return (0);
	}
	return (nb);
}

void	free_exec(t_exec *exec, char *path, char **command)
{
	free(path);
	ft_free_arr((void **)command);
	ft_free_arr((void **)exec->cmds);
	ft_free_arr((void **)exec->tokens);
	free(exec->old_token);
	ft_free_arr((void **)exec->command);
	free(exec->pid);
	free(exec);
}
