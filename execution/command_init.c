/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 22:59:29 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:29:51 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	**ft_split_path(t_env *env)
{
	char	**path;
	t_env	*tmp;

	tmp = env;
	while (tmp && ft_strcmp(tmp->var_name, "PATH"))
		tmp = tmp->next;
	path = NULL;
	if (tmp)
		path = ft_split(tmp->value, ':');
	return (path);
}

static char	*ft_get_path(char *command, char **splited_path, int flag, int acs)
{
	char	*path;

	if (!acs)
		return (ft_strdup(command));
	if (!splited_path)
	{
		perror(command);
		return (NULL);
	}
	command = ft_strjoin("/", command);
	while (splited_path && splited_path[flag] && acs == -1)
	{
		path = ft_strjoin(splited_path[flag++], command);
		acs = access(path, X_OK);
		if (acs == -1)
			free(path);
	}
	free(command);
	if (acs == -1)
		return (NULL);
	return (path);
}

char	*ft_get_path_of_cmd(char **command, int *tokens, t_env *env)
{
	char	*path_of_cmd;
	char	**path;
	int		i;
	int		acs;

	i = 0;
	while (command[i] && tokens[i] != CMD_NAME && tokens[i] != CMD_ARG)
		i++;
	if (!command[i] || !command[i][0])
		return (NULL);
	path = ft_split_path(env);
	acs = access(command[i], X_OK);
	path_of_cmd = ft_get_path(command[i], path, 0, acs);
	ft_free_arr((void **)path);
	return (path_of_cmd);
}

static int	ft_get_nbr_cmd(int *tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i] == CMD_ARG || tokens[i] == CMD_NAME)
			count++;
		i++;
	}
	return (count);
}

char	**ft_get_command(char **cmd_arg, int *tokens, t_env *env, char *cmd)
{
	char	**command;
	int		nbr_cmd;
	int		i;

	command = ft_split_path(env);
	if (!command && env && !cmd)
		return (NULL);
	ft_free_arr((void **)command);
	nbr_cmd = ft_get_nbr_cmd(tokens);
	if (!nbr_cmd)
		return (NULL);
	command = malloc(sizeof(char *) * (nbr_cmd + 1));
	if (!command)
		return (NULL);
	command[nbr_cmd] = NULL;
	i = -1;
	nbr_cmd = 0;
	while (cmd_arg[++i])
		if (tokens[i] == CMD_NAME || tokens[i] == CMD_ARG)
			command[nbr_cmd++] = ft_strdup(cmd_arg[i]);
	return (command);
}
