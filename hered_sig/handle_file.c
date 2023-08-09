/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:00:34 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/09 10:00:52 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	open_out_file(char *name, int mode)
{
	int	fd;
	int	open_mode;

	open_mode = O_TRUNC;
	if (mode == FILE_APP)
		open_mode = O_APPEND;
	fd = open(name, O_WRONLY | O_CREAT | open_mode, 0644);
	if (fd == -1)
	{
		ft_error_msg("minishell: ", STDERR_FILENO);
		perror(name);
	}
	return (fd);
}

static int	open_in_file(char *name, int mode)
{
	int	fd;

	if (mode == LIMIT_HEREDOC)
		return (0);
	fd = open(name, O_RDONLY);
	if (fd == -1)
	{
		ft_error_msg("minishell: ", STDERR_FILENO);
		perror(name);
	}
	return (fd);
}

static void	get_file(t_exec *exec, int nbr_command, int pipe_fd[2][2], int fd[2], int flag)
{
	fd[0] = pipe_fd[(nbr_command + 1) % 2][0];
	fd[1] = pipe_fd[nbr_command % 2][1];
	while (exec->cmds[nbr_command][++flag] && fd[0] != -1 && fd[1] != -1)
	{
		if (exec->tokens[nbr_command][flag] == FILE_OUTPUT
			|| exec->tokens[nbr_command][flag] == FILE_APP)
		{
			close_fd(fd[1]);
			fd[1] = open_out_file(exec->cmds[nbr_command][flag],
					exec->tokens[nbr_command][flag]);
		}
		else if (exec->tokens[nbr_command][flag] == FILE_INPUT)
		{
			close_fd(fd[0]);
			fd[0] = open_in_file(exec->cmds[nbr_command][flag],
					exec->tokens[nbr_command][flag]);
		}
	}
	if (fd[0] != -1 && fd[1] != -1)
		fd[0] = check_use_heredoc(fd[0], exec, nbr_command);
	else
		close_unused_hd(exec->heredoc_fd, nbr_command);
	pipe_fd[(nbr_command + 1) % 2][0] = fd[0];
	pipe_fd[nbr_command % 2][1] = fd[1];
}

void	handle_redirection(t_exec *exec, int nbr_command)
{
	int	fd[2];

	if (exec->count_cmd == 1 && nbr_command == 0)
	{
		exec->pipe_fd[1][0] = dup(STDIN_FILENO);
		exec->pipe_fd[0][1] = dup(STDOUT_FILENO);
		get_file(exec, nbr_command, exec->pipe_fd, fd, -1);
		return ;
	}
	if (nbr_command == 0)
	{
		pipe(exec->pipe_fd[0]);
		exec->pipe_fd[1][0] = dup(STDIN_FILENO);
		get_file(exec, nbr_command, exec->pipe_fd, fd, -1);
		return ;
	}
	if (nbr_command == exec->count_cmd - 1)
	{
		exec->pipe_fd[nbr_command % 2][1] = dup(STDOUT_FILENO);
		get_file(exec, nbr_command, exec->pipe_fd, fd, -1);
		return ;
	}
	pipe(exec->pipe_fd[nbr_command % 2]);
	get_file(exec, nbr_command, exec->pipe_fd, fd, -1);
}
