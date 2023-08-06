/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:23:19 by ataouaf           #+#    #+#             */
/*   Updated: 2023/07/30 15:10:19 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_is_wtspc(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

static int	lenght_next_cmd(char *command)
{
	int	i;
	int	state;

	state = 0;
	i = 0;
	while (command && command[i])
	{
		if (command[i] == '\'' || command[i] == '\"')
			ft_check_state(&state, command[i]);
		i++;
		if (ft_is_wtspc(command[i]) && !state)
			break ;
	}
	return (i);
}

static int	cmd_count(char *command)
{
	int	i;
	int	nb;
	int	state;

	state = 0;
	nb = 0;
	i = 0;
	while (command && command[i])
	{
		if (command[i] == '\'' || command[i] == '\"')
			ft_check_state(&state, command[i]);
		i++;
		if (ft_is_wtspc(command[i]) && !state)
		{
			while (command[i] && ft_is_wtspc(command[i]))
				i++;
			if (command[i])
				nb++;
		}
	}
	return (nb + (state == 0));
}

char	**ft_split_cmd(char *command)
{
	char	**cmd;
	int		nb_cmd;
	int		len_cmd;
	int		i;

	while (command && *command && ft_is_wtspc(*command))
		command++;
	nb_cmd = cmd_count(command);
	cmd = malloc((nb_cmd + 1) * sizeof(char *));
	cmd[nb_cmd] = NULL;
	i = 0;
	while (i < nb_cmd)
	{
		len_cmd = lenght_next_cmd(command);
		cmd[i] = ft_substr(command, 0, len_cmd);
		command += len_cmd + (command[len_cmd] != '\0');
		while (*command && ft_is_wtspc(*command))
			command++;
		i++;
	}
	return (cmd);
}
