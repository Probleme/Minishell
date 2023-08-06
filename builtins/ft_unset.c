/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:31:48 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:34:06 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_isalnum(int c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}

static int	ft_parse_env_name(const char *command, int *flag)
{
	int	i;

	if (!ft_isalpha(command[0]) && command[0] != '_')
	{
		*flag = 1;
		dprintf(2, "minishell: unset: `%s': not a valid identifier\n", command);
		return (0);
	}
	i = 0;
	while (command[i])
	{
		if (!ft_isalnum(command[i]) && command[i] != '_')
		{
			*flag = 1;
			dprintf(2,
				"minishell: unset: `%s': not a valid identifier\n", command);
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_unset(t_env **env, char **command)
{
	int		i;
	int		exit_code;
	t_env	*delete;

	i = 1;
	exit_code = 0;
	while (command[i])
	{
		if (ft_parse_env_name(command[i], &exit_code))
		{
			g_exit_status = 0;
			delete = ft_list_search(*env, command[i]);
			if (delete)
				ft_list_clearone(env, delete);
		}
		i++;
	}
	if (exit_code == 1)
		g_exit_status = 1;
}
