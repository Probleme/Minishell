/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 21:16:08 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/09 14:54:33 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	herd_arr_init(int arr_herd[16][2])
{
	int	i;

	i = -1;
	while (++i < 16)
	{
		arr_herd[i][0] = -1;
		arr_herd[i][1] = -1;
	}
}

// static void handle_herd_cmd(char *line, t_exec *exec, char *limit, int fd, int i)
// {
//     if (!line)
// 	{
//         ft_error_msg("\n", STDERR_FILENO);
// 		return;
// 	}
//     exec->herd_cmd = ft_split_cmd(line);
//     if (!exec->herd_cmd)
// 	{
//         ft_error_msg("\n", STDERR_FILENO);
//         return;
//     }
//     handle_dollar(exec->herd_cmd, NULL, *exec->env);
//     delete_quotes(exec->herd_cmd);
//     while (exec->herd_cmd[++i] && ft_strcmp(exec->herd_cmd[i], limit))
// 	{
//         ft_printf(NULL, NULL, exec->herd_cmd[i], fd);
//         free(line);
//         line = readline("heredoc > ");
//         exec->herd_cmd = ft_split_cmd(line);
//         handle_dollar(exec->herd_cmd, NULL, *exec->env);
//         delete_quotes(exec->herd_cmd);
//         i = -1;
//     }
//     if (!state_stdinput())
//         ft_error_msg("\n", STDOUT_FILENO);
//     free(line);
//     handle_signal(DEFAULT_SIGNAL);
// }

// static void read_from_stdin(char *limit, int fd, t_exec *exec) {
//     char *line;

//     handle_signal(HEREDOC_SIGNAL);
//     line = readline("heredoc > ");
    
//     handle_herd_cmd(line, exec, limit, fd, -1);

//     return;
// }


static void	read_from_stdin(char *limit, int fd, t_exec *exec, int i)
{
	char	*line;

	handle_signal(HEREDOC_SIGNAL);
	line = readline("heredoc > ");
	exec->herd_cmd = ft_split_cmd(line);
	handle_dollar(exec->herd_cmd, NULL, *exec->env);
	delete_quotes(exec->herd_cmd);
	while (exec->herd_cmd[++i] && ft_strcmp(exec->herd_cmd[i], limit))
	{
		ft_printf(NULL, NULL, exec->herd_cmd[i], fd);
		free(line);
		line = readline("heredoc > ");
		if (!line)
			break ;
		exec->herd_cmd = ft_split_cmd(line);
		handle_dollar(exec->herd_cmd, NULL, *exec->env);
		delete_quotes(exec->herd_cmd);
		i = -1;
	}
	if (!state_stdinput())
		ft_error_msg("\n", STDOUT_FILENO);
	free(line);
	handle_signal(DEFAULT_SIGNAL);
	return ;
}

static int	heredoc_read(char *limit,t_exec *exec)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	read_from_stdin(limit, fd[1], exec, -1);
	close(fd[1]);
	return (fd[0]);
}

static void	open_heredoc(int arr_herd[16][2], char ***commands, int **tokens, t_exec *exec)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	k = 0;
	herd_arr_init(arr_herd);
	while (commands[++i] && state_stdinput())
	{
		j = 0;
		while (commands[i][j] && state_stdinput())
		{
			if (tokens[i][j] == LIMIT_HEREDOC)
			{
				if (arr_herd[k][0] == i)
					close_fd(arr_herd[k][1]);
				arr_herd[k][1] = heredoc_read(commands[i][j], exec);
				arr_herd[k][0] = i;
			}
			j++;
		}
		if (arr_herd[k][0] == i)
			k++;
	}
}

int	init_heredoc(int arr_herd[16][2], char ***commands, int **tokens, t_exec *exec)
{
	int	stdin_var;

	stdin_var = dup(STDIN_FILENO);
	open_heredoc(arr_herd, commands, tokens, exec);
	if (!state_stdinput())
	{
		dup2(stdin_var, STDIN_FILENO);
		close(stdin_var);
		heredoc_close(arr_herd);
		return (1);
	}
	close(stdin_var);
	return (0);
}
