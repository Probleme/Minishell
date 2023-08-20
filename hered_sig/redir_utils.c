/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 14:56:52 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 15:02:55 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	handle_error(const char *filename)
{
	ft_dprintf(STDERR_FILENO, "minishell: ");
	perror(filename);
}

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
		handle_error(name);
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
		handle_error(name);
	}
	return (fd);
}

void	handle_out_file(int fd[], t_exec *exec, int nbr_command, int flag)
{
	close_fd(fd[1]);
	fd[1] = open_out_file(exec->cmds[nbr_command][flag],
		exec->tokens[nbr_command][flag]);
}

void	handle_in_file(int fd[], t_exec *exec, int nbr_command, int flag)
{
	close_fd(fd[0]);
	fd[0] = open_in_file(exec->cmds[nbr_command][flag],
		exec->tokens[nbr_command][flag]);
}
