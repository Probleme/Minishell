/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 22:59:29 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/17 11:08:51 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"
// int ft_strarr_contains(char **arr, char *str)
// {
//     if (arr == NULL || str == NULL)
//         return 0;

//     for (int i = 0; arr[i] != NULL; i++)
//     {
//         if (arr[i] != NULL && ft_strcmp(arr[i], str) == 0)
//             return 1;
//     }
//     return 0;
// }

// static char **ft_split_path(t_env *env)
// {
//     if (env == NULL) 
//         return NULL;

//     char **path;
//     int path_size = 0;
//     t_env *tmp = env;

//     while (tmp && ft_strcmp(tmp->var_name, "PATH"))
//         tmp = tmp->next;
    
//     if (tmp && tmp->value)
//         path_size = ft_count_words(tmp->value, ':') + 1;

//     path = malloc(sizeof(char *) * (path_size + 2)); // add +1 more to fit _PATH_STDPATH
      
//     if (!path)
//         return NULL;

//     for (int i = 0; i < path_size + 2; i++)
//         path[i] = NULL; // initialize to NULL

//     int index = 0;
//     if (tmp && tmp->value)
//     {
//         char **split_path = ft_split(tmp->value, ':');

//         if (!split_path)
//         {
//             free(path);
//             return NULL;
//         }

//         for (int i = 0; split_path[i]; i++)
//         {
//             path[index] = ft_strdup(split_path[i]);
//             index++;
//         }
      
//         ft_free_arr((void **)split_path);
//     }
//     if (!ft_strarr_contains(path, _PATH_STDPATH) && _PATH_STDPATH)
//     {
//         path[index] = ft_strdup(_PATH_STDPATH);
//         index++;
//     }
//     path[index] = NULL;
//     return path;
// }

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

static char	*ft_get_path(char *command, char **splited_path, int flag, int acs, t_exec *exec)
{
	char	*path;

	if (!acs)
		return (ft_strdup(command));
	if (!splited_path)
	{
		exec->flag = 1;
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

char	*ft_get_path_of_cmd(char **command, int *tokens, t_env *env, t_exec *exec)
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
	exec->flag = 0;
	path_of_cmd = ft_get_path(command[i], path, 0, acs, exec);
	if (!path_of_cmd)
		return (NULL);
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
