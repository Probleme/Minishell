/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 02:19:34 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/06 18:20:42 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	check_dir(char *cmd)
{
	DIR	*dirp;

	if (!cmd)
		return (0);
	dirp = opendir(cmd);
	if (dirp)
	{
		dprintf(2, "minishell: %s: Is a directory\n", cmd);
		g_exit_status = 126;
		closedir(dirp);
		return (1);
	}
	return (0);
}

static char	**ft_transform_envp(t_env *env)
{
	int		i;
	char	*tmp;
	char	**transformed_env;

	transformed_env = malloc(sizeof(char *) * (ft_list_size(env) + 1));
	i = 0;
	while (env != NULL)
	{
		if (env->value)
		{
			tmp = ft_strjoin(env->var_name, "=");
			transformed_env[i] = ft_strjoin(tmp, env->value);
			free(tmp);
		}
		else
			transformed_env[i] = ft_strdup(env->var_name);
		env = env->next;
		i++;
	}
	transformed_env[i] = NULL;
	return (transformed_env);
}

void	ft_execute_cmd(char *path, char **command, int nbr_cmd, t_exec *exec)
{
	char	**envp;

	if (exec->pipe_fd[(nbr_cmd + 1) % 2][0] == -1 || exec->pipe_fd[nbr_cmd
		% 2][1] == -1 || !command || check_dir(command[0]) || check_path(exec))
		return ;
	if (path == NULL && command[0])
	{
		dprintf(2, "minishell: %s: command not found\n", command[0]);
		return ;
	}
	handle_signal(PARENT_SIGNAL);
	exec->pid[nbr_cmd] = fork();
	if (exec->pid[nbr_cmd] == 0)
	{
		handle_signal(CHILD_SIGNAL);
		dup2(exec->pipe_fd[(nbr_cmd + 1) % 2][0], STDIN_FILENO);
		dup2(exec->pipe_fd[nbr_cmd % 2][1], STDOUT_FILENO);
		close_all_fd(exec->pipe_fd, nbr_cmd);
		envp = ft_transform_envp(*exec->env);
		execve(path, command, envp);
		free_env_list(exec->env);
		ft_free_arr((void **)envp);
		free_exec(exec, path, command);
		exit(1);
	}
}

void	ft_wait_children(int *pid1, int nbr_cmd)
{
	int	pid;
	int	status;

	pid = 0;
	while (pid < nbr_cmd)
	{
		if (pid1[pid] != -1)
		{
			waitpid(pid1[pid], &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == 2)
					g_exit_status = 130;
				else if (WTERMSIG(status) == 3)
					g_exit_status = 131;
				else if (WTERMSIG(status) == 11)
					g_exit_status = 139;
			}
		}
		pid++;
	}
}
