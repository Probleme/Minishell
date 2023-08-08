/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:31:48 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 12:10:43 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_isalnum(int c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}

void	ft_printf(char *str1, char *str2, char *cmd, int fd)
{
	if (str1)
		write(fd, str1, ft_strlen(str1));
	if (cmd)
		write(fd, cmd, ft_strlen(cmd));
	if (str2)
		write(fd, str2, ft_strlen(str2));
	write(fd, "\n", 1);
}

static int	ft_parse_env_name(char *command, int *flag)
{
	int	i;

	if (!ft_isalpha(command[0]) && command[0] != '_')
	{
		*flag = 1;
		ft_printf("minishell: unset: `", "': not a valid identifier", command, 2);
		return (0);
	}
	i = 0;
	while (command[i])
	{
		if (!ft_isalnum(command[i]) && command[i] != '_')
		{
			*flag = 1;
			ft_printf("minishell: unset: `", "': not a valid identifier", command, 2);
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
