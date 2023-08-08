/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 23:06:29 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 12:13:07 by ataouaf          ###   ########.fr       */
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

static void	ft_exit_more(char **command, t_exec *exec)
{
	if (ft_is_digit(command[1]))
	{
		ft_error_msg("exit\n", STDOUT_FILENO);
		ft_error_msg("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_exit_status = 1;
	}
	else
	{
		ft_error_msg("exit\n", STDOUT_FILENO);
		ft_printf("minishell: exit: ", "too many arguments", command[1], STDERR_FILENO);
		free_env_list(exec->env);
		free_exec(exec, NULL, command);
		exit (2);
	}
}

static void	ft_exit_two(char **command, t_exec *exec)
{
	(void)exec;
	unsigned char	stat;

	if (ft_is_digit(command[1]))
	{
		ft_error_msg("exit\n", STDOUT_FILENO);
		free_env_list(exec->env);
		stat = ft_atoll(command[1]);
		free_exec(exec, NULL, command);
		exit (stat);
	}
	else
	{
		ft_error_msg("exit\n", STDOUT_FILENO);
		ft_printf("minishell: exit: ", "numeric argument required", NULL, STDERR_FILENO);
		free_env_list(exec->env);
		free_exec(exec, NULL, command);
		exit (255);
	}
}

void	ft_exit(char **command, t_exec *exec)
{
	if (ft_sizeof_array(command) == 1)
	{
		ft_error_msg("exit\n", STDOUT_FILENO);
		free_env_list(exec->env);
		free_exec(exec, NULL, command);
		exit (g_exit_status);
	}
	else if (ft_sizeof_array(command) == 2)
		ft_exit_two(command, exec);
	else
		ft_exit_more(command, exec);
}
