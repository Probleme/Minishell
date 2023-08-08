/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 23:44:34 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 14:17:22 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

char	*ft_strjoin_add_space(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	new = ft_calloc(sizeof(char), (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!new)
		return (0);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = 32;
	i++;
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	return (new);
}

static int	contain_only_n(const char *str)
{
	int	i;

	if (!str || !str[0] || !str[1])
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	execute_echo_noflag(char **array_str)
{
	int		i;
	char	*tmp;
	char	*output;

	i = 1;
	output = ft_strdup(array_str[1]);
	while (array_str[i + 1])
	{
		tmp = ft_strdup(output);
		free(output);
		output = ft_strjoin_add_space(tmp, array_str[i + 1]);
		free(tmp);
		i++;
	}
	printf("%s\n", output);
	free(output);
	return ;
}

static void	execute_echo_withflag(char **array_str, int size)
{
	int		i;
	char	*tmp;
	char	*output;

	i = 1;
	while (contain_only_n(array_str[i]))
	{
		if (i == size - 1)
			return ;
		i++;
	}
	output = ft_strdup(array_str[i]);
	while (array_str[i + 1])
	{
		tmp = ft_strdup(output);
		free(output);
		output = ft_strjoin_add_space(tmp, array_str[i + 1]);
		free(tmp);
		i++;
	}
	printf("%s", output);
	free(output);
	return ;
}

void	ft_echo(char **command)
{
	int	size;

	g_exit_status = 0;
	size = ft_sizeof_array(command);
	if (size == 1)
	{
		printf("\n");
		return ;
	}
	if (command[1][0] != '-' || (command[1][0] == '-'
			&& ft_strlen(command[1]) == 1) || !contain_only_n(command[1]))
		execute_echo_noflag(command);
	else if (contain_only_n(command[1]))
		execute_echo_withflag(command, size);
}
