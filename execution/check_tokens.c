/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:12:40 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 11:23:27 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_check_hered_max(int *cmd_type)
{
	int	i;
	int	nbr_heredoc;

	nbr_heredoc = 0;
	i = 0;
	while (cmd_type[i])
	{
		if (cmd_type[i] == SIGNAL_HEREDOC)
			nbr_heredoc += 1;
		i++;
	}
	if (nbr_heredoc > 16)
	{
		g_exit_status = 2;
		ft_error_msg("minishell: maximum here-document exceeded\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

static int	ft_check_operator_file(int *type_cmd)
{
	int	i;

	i = 0;
	while (type_cmd[i])
	{
		if (type_cmd[i] == SIGNAL_INPUT && type_cmd[i + 1] != FILE_INPUT)
			return (0);
		if (type_cmd[i] == SIGNAL_OUTPUT && type_cmd[i + 1] != FILE_OUTPUT)
			return (0);
		if (type_cmd[i] == SIGNAL_APP && type_cmd[i + 1] != FILE_APP)
			return (0);
		if (type_cmd[i] == SIGNAL_HEREDOC && type_cmd[i + 1] != LIMIT_HEREDOC)
			return (0);
		i++;
	}
	return (1);
}

static int	ft_check_double_pipe(int *type_cmd)
{
	int	i;

	i = 0;
	while (type_cmd[i])
	{
		if (type_cmd[i] == PIPE && type_cmd[i + 1] == PIPE)
		{
			ft_error_msg("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_check_input(int *type_cmd)
{
	if (!ft_check_double_pipe(type_cmd))
	{
		g_exit_status = 2;
		return (0);
	}
	if (!ft_check_operator_file(type_cmd))
	{
		g_exit_status = 2;
		ft_error_msg("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (0);
	}
	if (!ft_check_hered_max(type_cmd))
		return (0);
	return (1);
}
