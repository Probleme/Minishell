/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 21:16:08 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 12:24:21 by ataouaf          ###   ########.fr       */
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

static void	read_from_stdin(char *limit, int fd)
{
	char	*line;

	handle_signal(HEREDOC_SIGNAL);
	line = readline("heredoc > ");
	while (line && ft_strcmp(line, limit))
	{
		ft_printf(NULL, NULL, line, fd);
		free(line);
		line = readline("heredoc > ");
	}
	if (!state_stdinput())
		ft_error_msg("\n", STDERR_FILENO);
	else if (!line)
		ft_error_msg("\n", STDERR_FILENO);
	free(line);
	handle_signal(DEFAULT_SIGNAL);
	return ;
}

static int	heredoc_read(char *limit)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	read_from_stdin(limit, fd[1]);
	close(fd[1]);
	return (fd[0]);
}

static void	open_heredoc(int arr_herd[16][2], char ***commands, int **tokens)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	herd_arr_init(arr_herd);
	while (commands[i] && state_stdinput())
	{
		j = 0;
		while (commands[i][j] && state_stdinput())
		{
			if (tokens[i][j] == LIMIT_HEREDOC)
			{
				if (arr_herd[k][0] == i)
					close_fd(arr_herd[k][1]);
				arr_herd[k][1] = heredoc_read(commands[i][j]);
				arr_herd[k][0] = i;
			}
			j++;
		}
		if (arr_herd[k][0] == i)
			k++;
		i++;
	}
}

int	init_heredoc(int arr_herd[16][2], char ***commands, int **tokens)
{
	int	stdin_var;

	stdin_var = dup(STDIN_FILENO);
	open_heredoc(arr_herd, commands, tokens);
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
