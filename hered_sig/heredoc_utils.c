/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 09:10:59 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 17:41:44 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	state_stdinput(void)
{
	int	test;

	test = dup(STDIN_FILENO);
	if (test == -1)
		return (0);
	close(test);
	return (1);
}

void	heredoc_close(int arr_heredoc[16][2])
{
	int	i;

	i = -1;
	while (++i < 15)
		if (arr_heredoc[i][0] != -1)
			close_fd(arr_heredoc[i][1]);
}

static int	index_last_heredoc(int *tokens)
{
	int	i;
	int	index;

	i = 0;
	index = -1;
	while (tokens[i])
	{
		if (tokens[i] == LIMIT_HEREDOC)
			index = i;
		i++;
	}
	return (index);
}

static int	get_fd_heredoc(int arr_heredoc[16][2], int nbr_cmd)
{
	int	i;

	i = 0;
	while (i < 16 && arr_heredoc[i][0] != -1)
	{
		if (arr_heredoc[i][0] == nbr_cmd)
			return (arr_heredoc[i][1]);
		i++;
	}
	return (-1);
}

int	check_use_heredoc(int fd, t_exec *exec, int nbr_cmd)
{
	int	i;

	i = index_last_heredoc(exec->tokens[nbr_cmd]);
	if (i == -1)
		return (fd);
	while (exec->tokens[nbr_cmd][i])
	{
		if (exec->tokens[nbr_cmd][i] == FILE_INPUT)
		{
			close_fd(get_fd_heredoc(exec->heredoc_fd, nbr_cmd));
			return (fd);
		}
		i++;
	}
	i = get_fd_heredoc(exec->heredoc_fd, nbr_cmd);
	if (i == -1)
		return (fd);
	else
	{
		close_fd(fd);
		return (i);
	}
}
