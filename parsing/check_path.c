/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:20:49 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/16 15:41:15 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	file_check(t_exec *exec)
{
	struct stat	statbuff;

	if (stat(exec->command[0], &statbuff) == -1)
	{
		ft_dprintf(2, "minishell: %s: No such file or directory\n", exec->command[0]);
		// custom_print(exec->command[0], ": No such file or directory", 2);
		g_exit_status = 127;
		return (1);
	}
	else if (statbuff.st_mode & S_IFDIR)
	{
		ft_dprintf(2, "minishell: %s: is a directory\n", exec->command[0]);
		// custom_print(exec->command[0], ": is a directory", 2);
		g_exit_status = 126;
		return (1);
	}
	else if ((statbuff.st_mode & S_IXUSR) == 0)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", exec->command[0]);
		// custom_print(exec->command[0], ": Permission denied", 2);
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