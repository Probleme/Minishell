/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 14:05:42 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:35:45 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_is_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static void	define_operator(char *command, int *cmd_type)
{
	if (command[0] == '|')
		*cmd_type = PIPE;
	else if (ft_strlen(command) > 1 && command[0] == '<')
		*cmd_type = SIGNAL_HEREDOC;
	else if (ft_strlen(command) > 1 && command[0] == '>')
		*cmd_type = SIGNAL_APP;
	else if (command[0] == '<')
		*cmd_type = SIGNAL_INPUT;
	else if (command[0] == '>')
		*cmd_type = SIGNAL_OUTPUT;
}

static void	token_type(char **command, int **type_cmd, int i, int *is_pipe)
{
	if (ft_is_charset(command[i][0], "<>|"))
	{
		define_operator(command[i], &(*type_cmd)[i]);
		if ((*type_cmd)[i] == PIPE)
			*is_pipe = 0;
	}
	else if (i > 0 && (*type_cmd)[i - 1] == SIGNAL_HEREDOC)
		(*type_cmd)[i] = LIMIT_HEREDOC;
	else if (i > 0 && (*type_cmd)[i - 1] == SIGNAL_INPUT)
		(*type_cmd)[i] = FILE_INPUT;
	else if (i > 0 && (*type_cmd)[i - 1] == SIGNAL_OUTPUT)
		(*type_cmd)[i] = FILE_OUTPUT;
	else if (i > 0 && (*type_cmd)[i - 1] == SIGNAL_APP)
		(*type_cmd)[i] = FILE_APP;
	else if ((i > 0 && (*type_cmd)[i - 1] == CMD_NAME) || (i > 0
			&& (*type_cmd)[i - 1] == CMD_ARG) || *is_pipe == 1)
		(*type_cmd)[i] = CMD_ARG;
	else
	{
		(*type_cmd)[i] = CMD_NAME;
		*is_pipe = 1;
	}
}

int	*set_type_tokens(char **command)
{
	int	*type_cmd;
	int	is_pipe;
	int	i;

	i = 0;
	is_pipe = 0;
	type_cmd = ft_calloc(sizeof(int), ft_sizeof_array(command) + 1);
	if (!type_cmd)
		return (NULL);
	while (command[i])
	{
		token_type(command, &type_cmd, i, &is_pipe);
		i++;
	}
	if (ft_check_input(type_cmd))
		return (type_cmd);
	g_exit_status = 2;
	free(type_cmd);
	return (NULL);
}

void	close_fd(int fd)
{
	if (fd != -1)
		close(fd);
}
