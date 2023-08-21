/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 02:19:34 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 18:24:17 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	ft_pwd(t_env *env)
{
	char	pwd[4096];
	t_env	*pwd_env_node;
	t_env	*oldpwd_env_node;

	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		pwd_env_node = ft_list_search(env, "PWD");
		oldpwd_env_node = ft_list_search(env, "OLDPWD");
		if (pwd_env_node && pwd_env_node->value)
			ft_dprintf(1, "%s\n", pwd_env_node->value);
		else if (oldpwd_env_node && oldpwd_env_node->value)
			ft_dprintf(1, "%s\n", oldpwd_env_node->value);
		else
			ft_dprintf(2,
				"minishell: pwd: error retrieving current directory: getcwd:\
				 cannot access parent directories: No such file or directory\n");
		g_exit_status = 0;
		return ;
	}
	else
		ft_dprintf(1, "%s\n", pwd);
	g_exit_status = 0;
}

static int	check_dir(char *cmd)
{
	DIR	*dirp;

	if (!cmd)
		return (0);
	dirp = opendir(cmd);
	if (dirp)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: Is a directory\n", cmd);
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
	if (!transformed_env)
		return (NULL);
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
		% 2][1] == -1 || !command || check_dir(command[0]) || check_path(exec)
		|| ft_check_path(exec, *exec->env))
		return ;
	if (path == NULL && command[0] != NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			command[0]);
		g_exit_status = 127;
		return ;
	}
	// if (exec->count_cmd > 1)
	// {
	// 	free(path);
	// 	path = NULL;
	// 	ft_free_arr((void **)command);
	// }
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
