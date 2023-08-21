/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:20:49 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 16:27:10 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	file_check(t_exec *exec)
{
	struct stat	statbuff;

	if (stat(exec->command[0], &statbuff) == -1)
	{
		ft_dprintf(2, "minishell: %s: No such file or directory\n",
			exec->command[0]);
		g_exit_status = 127;
		return (1);
	}
	else if (statbuff.st_mode & S_IFDIR)
	{
		ft_dprintf(2, "minishell: %s: is a directory\n", exec->command[0]);
		g_exit_status = 126;
		return (1);
	}
	else if ((statbuff.st_mode & S_IXUSR) == 0)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", exec->command[0]);
		g_exit_status = 126;
		return (1);
	}
	return (0);
}

int	check_path(t_exec *exec)
{
	if (ft_strchr(exec->command[0], '/') || exec->command[0][0] == '.')
		if (file_check(exec))
			return (1);
	return (0);
}

int	ft_check_path(char **command, t_env *env, int nbr_cmd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, "PATH") == 0 && (tmp->value == NULL))
		{
			ft_dprintf(STDERR_FILENO, "env: No such file or directory\n");
			g_exit_status = 127;
			return (1);
		}
		if (ft_strcmp(tmp->var_name, "PATH") == 0 && (strncmp(tmp->value,
					_PATH_STDPATH, sizeof(_PATH_STDPATH))) != 0)
		{
			ft_free_arr((void **)command);
			ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
				command[nbr_cmd]);
			g_exit_status = 127;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
