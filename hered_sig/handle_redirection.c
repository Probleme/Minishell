/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:00:34 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/21 23:46:35 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	get_file(t_exec *exec, int nbr_command, int pipe_fd[2][2],
		int fd[2])
{
	int	flag;

	flag = -1;
	fd[0] = pipe_fd[(nbr_command + 1) % 2][0];
	fd[1] = pipe_fd[nbr_command % 2][1];
	while (exec->cmds[nbr_command][++flag] && fd[0] != -1 && fd[1] != -1)
	{
		if (exec->tokens[nbr_command][flag] == FILE_OUTPUT
			|| exec->tokens[nbr_command][flag] == FILE_APP)
			handle_out_file(fd, exec, nbr_command, flag);
		else if (exec->tokens[nbr_command][flag] == FILE_INPUT)
			handle_in_file(fd, exec, nbr_command, flag);
	}
	if (fd[0] != -1 && fd[1] != -1)
		fd[0] = check_use_heredoc(fd[0], exec, nbr_command);
	else
		close_unused_hd(exec->heredoc_fd, nbr_command);
	pipe_fd[(nbr_command + 1) % 2][0] = fd[0];
	pipe_fd[nbr_command % 2][1] = fd[1];
}

void	handle_first_command(t_exec *exec, int fd[2])
{
	pipe(exec->pipe_fd[0]);
	exec->pipe_fd[1][0] = dup(STDIN_FILENO);
	get_file(exec, 0, exec->pipe_fd, fd);
}

void	handle_last_command(t_exec *exec, int nbr_command, int fd[2])
{
	exec->pipe_fd[nbr_command % 2][1] = dup(STDOUT_FILENO);
	get_file(exec, nbr_command, exec->pipe_fd, fd);
}

void	handle_normal_command(t_exec *exec, int nbr_command, int fd[2])
{
	pipe(exec->pipe_fd[nbr_command % 2]);
	get_file(exec, nbr_command, exec->pipe_fd, fd);
}

void	handle_redirection(t_exec *exec, int nbr_command)
{
	int	fd[2];

	if (exec->count_cmd == 1 && nbr_command == 0)
	{
		exec->pipe_fd[1][0] = dup(STDIN_FILENO);
		exec->pipe_fd[0][1] = dup(STDOUT_FILENO);
		get_file(exec, nbr_command, exec->pipe_fd, fd);
	}
	else if (nbr_command == 0)
		handle_first_command(exec, fd);
	else if (nbr_command == exec->count_cmd - 1)
		handle_last_command(exec, nbr_command, fd);
	else
		handle_normal_command(exec, nbr_command, fd);
}
