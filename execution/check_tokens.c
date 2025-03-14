/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 23:12:40 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/23 18:23:12 by ataouaf          ###   ########.fr       */
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
		g_exit_status = 258;
		ft_dprintf(STDERR_FILENO,
			"minishell: maximum here-document exceeded\n");
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

static int	ft_is_end_with_pipe(int *type_cmd)
{
	int	i;

	i = 0;
	while (type_cmd[i])
		i++;
	if (type_cmd[i - 1] && type_cmd[i - 1] == PIPE)
		return (1);
	return (0);
}

int	ft_check_input(int *type_cmd)
{
	if (!ft_check_operator_file(type_cmd) || ft_is_end_with_pipe(type_cmd))
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error\n");
		g_exit_status = 258;
		return (0);
	}
	if (!ft_check_hered_max(type_cmd))
		return (0);
	return (1);
}
