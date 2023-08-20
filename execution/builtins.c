/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:32:18 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 14:02:52 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_check_env(t_env *env, char **command, t_env *tmp)
{
	if (command != NULL && ft_sizeof_array(command) > 1)
	{
		ft_dprintf(STDERR_FILENO, "env: ‘%s‘: No such file or directory\n",
			command[1]);
		g_exit_status = 127;
		return (1);
	}
	if (!ft_list_search(env, "PATH"))
	{
		ft_dprintf(STDERR_FILENO, "env: No such file or directory\n");
		g_exit_status = 127;
		return (1);
	}
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, "PATH") == 0 && (tmp->value == NULL
				|| (strncmp(tmp->value, _PATH_STDPATH,
						sizeof(_PATH_STDPATH))) != 0))
		{
			ft_dprintf(STDERR_FILENO, "env: No such file or directory\n");
			g_exit_status = 127;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

static void	ft_env(t_env *env, char **command)
{
	if (ft_check_env(env, command, NULL))
		return ;
	while (env != NULL)
	{
		if (env->value)
		{
			if (env->var_name)
				ft_dprintf(1, "%s=%s\n", env->var_name, env->value);
			else
				ft_dprintf(1, "%s\n", env->value);
		}
		env = env->next;
	}
	g_exit_status = 0;
}

static void	begin_builtin(char **command, int *tokens, t_exec *exec, int flag)
{
	char	**str;

	str = ft_get_command(command, tokens, NULL, NULL);
	if (!ft_strcmp(str[0], "pwd"))
		ft_pwd(*exec->env);
	else if (!ft_strcmp(str[0], "echo"))
		ft_echo(str);
	else if (!ft_strcmp(str[0], "exit"))
	{
		close_fd(exec->pipe_fd[flag % 2][0]);
		ft_exit(str, exec);
	}
	else if (!ft_strcmp(str[0], "cd"))
		ft_cd(str, *exec->env, NULL);
	else if (!ft_strcmp(str[0], "env"))
		ft_env(*exec->env, str);
	else if (!ft_strcmp(str[0], "export"))
		ft_export(exec->env, str);
	else if (!ft_strcmp(str[0], "unset"))
		ft_unset(exec->env, str);
	ft_free_arr((void **)str);
}

void	init_builtin(char **command, int *tokens, t_exec *exec, int flag)
{
	if (exec->pipe_fd[(flag + 1) % 2][0] == -1 || exec->pipe_fd[flag % 2][1] ==
		-1)
		return ;
	exec->pid[flag] = fork();
	if (exec->pid[flag] == 0)
	{
		dup2(exec->pipe_fd[(flag + 1) % 2][0], STDIN_FILENO);
		dup2(exec->pipe_fd[flag % 2][1], STDOUT_FILENO);
		close_fd(exec->pipe_fd[(flag + 1) % 2][0]);
		close_fd(exec->pipe_fd[flag % 2][1]);
		begin_builtin(command, tokens, exec, flag);
		close_fd(exec->pipe_fd[flag % 2][0]);
		heredoc_close(exec->heredoc_fd);
		free_exec(exec, NULL, NULL);
		exit(g_exit_status);
	}
}

void	begin_builtin_env(char **command, int *tokens, t_exec *exec)
{
	char	**str;
	int		i;

	i = 0;
	if (!command || !tokens)
		return ;
	while (command[i] && tokens[i] != CMD_NAME)
		i++;
	if (!command[i])
		return ;
	str = ft_get_command(command, tokens, NULL, NULL);
	if (!ft_strcmp(command[i], "cd"))
		ft_cd(str, *exec->env, NULL);
	else if (!ft_strcmp(command[i], "export"))
		ft_export(exec->env, str);
	else if (!ft_strcmp(command[i], "unset"))
		ft_unset(exec->env, str);
	else if (!ft_strcmp(command[i], "exit"))
	{
		close(exec->pipe_fd[0][1]);
		close(exec->pipe_fd[1][0]);
		ft_exit(str, exec);
	}
	ft_free_arr((void **)str);
}
