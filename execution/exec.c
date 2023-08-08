/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:54:23 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 09:47:22 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_isbuiltin_env(char **command, int *tokens)
{
	int	i;

	i = 0;
	while (command && command[i] && tokens && tokens[i] != CMD_NAME)
		i++;
	if (!command[i])
		return (0);
	if (!ft_strcmp(command[i], "cd") || !ft_strcmp(command[i], "export")
		|| !ft_strcmp(command[i], "unset") || !ft_strcmp(command[i], "exit"))
		return (1);
	return (0);
}

int	ft_is_builtin(char **cmds, int *tokens)
{
	int	i;

	i = 0;
	while (cmds && cmds[i] && tokens && tokens[i] != CMD_NAME
		&& tokens[i] != CMD_ARG)
		i++;
	if (!cmds[i])
		return (0);
	if (!ft_strcmp(cmds[i], "echo") || !ft_strcmp(cmds[i], "env")
		|| !ft_strcmp(cmds[i], "pwd") || !ft_strcmp(cmds[i], "cd")
		|| !ft_strcmp(cmds[i], "export") || !ft_strcmp(cmds[i], "unset")
		|| !ft_strcmp(cmds[i], "exit"))
		return (1);
	return (0);
}

static void	loop_command(t_exec *exec, int flag, char *path, char **command)
{
	while (exec->cmds[flag])
	{
		handle_redirection(exec, flag);
		path = NULL;
		command = NULL;
		if (ft_is_builtin(exec->cmds[flag], exec->tokens[flag]))
			init_builtin(exec->cmds[flag], exec->tokens[flag], exec, flag);
		else
		{
			path = ft_get_path_of_cmd(exec->cmds[flag], exec->tokens[flag],
					*exec->env);
			command = ft_get_command(exec->cmds[flag], exec->tokens[flag],
					*exec->env, path);
			ft_execute_cmd(path, command, flag, exec);
			if (flag < exec->count_cmd - 1)
			{
				free(path);
				ft_free_arr((void **)command);
			}
		}
		close_fd(exec->pipe_fd[++flag % 2][0]);
		close_fd(exec->pipe_fd[(flag + 1) % 2][1]);
	}
	ft_wait_children(exec->pid, exec->count_cmd);
	ft_get_last_cmd(exec, flag - 1, path, command);
}

static void	begin_exec(t_exec *exec)
{
	exec->pipe_fd[0][0] = -1;
	exec->pipe_fd[0][1] = -1;
	exec->pipe_fd[1][0] = -1;
	exec->pipe_fd[1][1] = -1;
	if (check_path(exec))
		return ;
	if (ft_isbuiltin_env(exec->cmds[0], exec->tokens[0])
		&& exec->count_cmd == 1)
	{
		handle_redirection(exec, 0);
		begin_builtin_env(exec->cmds[0], exec->tokens[0], exec);
		close_fd(exec->pipe_fd[0][1]);
		close_fd(exec->pipe_fd[1][0]);
		return ;
	}
	else
		loop_command(exec, 0, NULL, NULL);
	handle_signal(DEFAULT_SIGNAL);
}

void	start_exec(t_env **env, char *command)
{
	t_exec	*exec;
	int		i;

	exec = init_execution(*env, command);
	if (!exec || !exec->old_token)
		return ;
	exec->env = env;
	exec->count_cmd = count_cmd(exec->command, exec->old_token);
	exec->cmds = ft_fill_commands(exec, exec->command);
	exec->tokens = count_tokens(exec, 0);
	exec->pid = malloc(sizeof(int) * (exec->count_cmd + 1));
	if (!exec->pid)
		return ;
	if (init_heredoc(exec->heredoc_fd, exec->cmds, exec->tokens))
		g_exit_status = 130;
	else if (exec->pid)
	{
		i = 0;
		while (i < exec->count_cmd)
			exec->pid[i++] = -1;
		exec->pid[i] = 0;
		begin_exec(exec);
	}
	free_exec(exec, NULL, NULL);
}
