/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:04:11 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/05 13:59:44 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

t_exec	*init_execution(t_env *env, char *command)
{
	t_exec	*exec;

	command = ft_operator_parse(command);
	if (!command)
		return (NULL);
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	exec->command = ft_split_cmd(command);
	free(command);
	if (!exec->command)
	{
		free(exec);
		return (NULL);
	}
	exec->old_token = set_type_tokens(exec->command);
	if (!exec->old_token)
	{
		ft_free_arr((void **)exec->command);
		free(exec);
		return (NULL);
	}
	handle_dollar(exec->command, exec->old_token, env);
	delete_quotes(exec->command);
	return (exec);
}

char	**ft_get_next_cmd(char **cmd_split, int *tokens)
{
	char	**next_cmd;
	int		lenght;
	int		i;

	lenght = 0;
	if (!cmd_split)
		return (NULL);
	while (cmd_split[lenght] && tokens[lenght] != PIPE)
		lenght++;
	next_cmd = malloc(sizeof(char *) * (lenght + 1));
	if (!next_cmd)
	{
		perror("malloc");
		return (NULL);
	}
	next_cmd[lenght] = NULL;
	i = 0;
	while (i < lenght)
	{
		next_cmd[i] = cmd_split[i];
		i++;
	}
	return (next_cmd);
}

char	***ft_fill_commands(t_exec *exec, char **command)
{
	char	***commands;
	int		i;
	int		j;

	commands = malloc(sizeof(char **) * (exec->count_cmd + 1));
	if (!commands)
	{
		perror("malloc");
		return (NULL);
	}
	commands[exec->count_cmd] = NULL;
	i = 0;
	j = 0;
	while (exec->command[i])
	{
		commands[j] = ft_get_next_cmd(&command[i], &exec->old_token[i]);
		i += ft_sizeof_array(commands[j]);
		if (exec->command[i])
			i++;
		j++;
	}
	return (commands);
}

int	**count_tokens(t_exec *exec, int flag)
{
	int	**arr_token;
	int	i;
	int	j;

	arr_token = malloc(sizeof(int *) * (exec->count_cmd + 1));
	if (!arr_token)
		return (NULL);
	arr_token[exec->count_cmd] = NULL;
	i = -1;
	while (++i < exec->count_cmd)
	{
		arr_token[i] = malloc(sizeof(int) * (ft_sizeof_array(exec->cmds[i])
					+ 1));
		j = 0;
		while (exec->old_token[flag + j] != 0 && exec->old_token[flag
				+ j] != PIPE)
		{
			arr_token[i][j] = exec->old_token[flag + j];
			j++;
		}
		arr_token[i][j] = 0;
		flag += j + 1;
	}
	return (arr_token);
}

void	ft_get_last_cmd(t_exec *exec, int flag, char *path, char **args)
{
	if (!path && !ft_is_builtin(exec->cmds[flag], exec->tokens[flag]) && args)
		g_exit_status = 127;
	if (exec->pipe_fd[flag % 2][1] == -1 || exec->pipe_fd[(flag + 1) % 2][0] ==
		-1)
		g_exit_status = 1;
	ft_free_arr((void **)args);
	free(path);
}
