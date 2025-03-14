/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 23:06:29 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 13:49:30 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_is_digit(char *str)
{
	int	i;
	int	error;

	i = 0;
	ft_erratoll(str, &error);
	if (error != 0)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			return (1);
		i++;
	}
	return (0);
}

static void	ft_exit_more(char **command)
{
	if (ft_is_digit(command[1]))
	{
		printf("exit\n");
		ft_dprintf(STDERR_FILENO, "minishell: exit: %s: too many arguments\n",
			command[1]);
		g_exit_status = 1;
	}
	else
	{
		printf("exit\n");
		ft_dprintf(STDERR_FILENO, "minishell: exit: \
			%s: numeric argument required\n", command[1]);
		exit(2);
	}
}

static void	ft_exit_two(char **command)
{
	unsigned char	stat;

	if (ft_is_digit(command[1]))
	{
		printf("exit\n");
		stat = ft_atoll(command[1]);
		exit(stat);
	}
	else
	{
		printf("exit\n");
		ft_dprintf(STDERR_FILENO, "minishell: exit: \
			%s: numeric argument required\n", command[1]);
		exit(255);
	}
}

void	ft_exit(char **command, t_exec *exec)
{
	if (ft_sizeof_array(command) == 1)
	{
		ft_dprintf(STDOUT_FILENO, "exit\n");
		free_env_list(exec->env);
		free_exec(exec, NULL, command);
		exit(g_exit_status);
	}
	else if (ft_sizeof_array(command) == 2)
		ft_exit_two(command);
	else
		ft_exit_more(command);
}
