/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:31:26 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/17 17:52:46 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

char	**ft_env_name(t_env *env)
{
	int		i;
	char	**env_name;

	i = 0;
	env_name = malloc(sizeof(char *) * (ft_list_size(env) + 1));
	if (!env_name)
	{
		perror("malloc");
		return (NULL);
	}
	while (env != NULL)
	{
		env_name[i] = ft_strdup(env->var_name);
		env = env->next;
		i++;
	}
	env_name[i] = NULL;
	return (env_name);
}

void	ft_update_path(t_env **env)
{
	char	*path;
	t_env	*path_node;

	path = ft_strdup(_PATH_STDPATH);
	path_node = ft_new_list("PATH");
	if (!path_node)
	{
		free(path);
		return ;
	}
	path_node->value = path;
	path_node->next = NULL;
	ft_list_add_back(env, path_node);
}

static void	ft_begin_export(char *command, t_env **env)
{
	if (strncmp(&command[0], "PATH", sizeof(command[0])) == 0 && !(ft_strchr(command, '=') && (*(ft_strchr(command, '=') - 1) == '+')))
	{
		if (ft_list_search(*env, "PATH"))
			ft_list_clearone(env, ft_list_search(*env, "PATH"));
		ft_update_path(env);
		return;
	}
	if (ft_strchr(command, '=') && (*(ft_strchr(command, '=') - 1) == '+'))
		ft_export_concatenate(command, env, NULL);
	else if (ft_strchr(command, '='))
		ft_export_assign(command, env);
	else
		ft_export_create_var(command, env);
}

static int	ft_parse_env(char *command, int *flag)
{
	int	i;

	if (!ft_isalpha(command[0]) && command[0] != '_')
	{
		*flag = 1;
		ft_dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n",
			command);
		return (0);
	}
	i = -1;
	while (command[++i])
	{
		if (command[i] == '=' || (command[i] == '+' && command[i + 1] == '='))
			break ;
		if (!ft_isalnum(command[i]) && command[i] != '_')
		{
			*flag = 1;
			ft_dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n",
				command);
			return (0);
		}
	}
	return (1);
}

void	ft_export(t_env **env, char **command)
{
	int	i;
	int	flag;
	
	if (ft_sizeof_array(command) == 1)
	{
		ft_print_env_var(*env);
		return ;
	}
	i = 1;
	flag = 0;
	while (command[i])
	{
		 if (ft_parse_env(command[i], &flag))
			ft_begin_export(command[i], env);
		else
			flag = 1;
		i++;
	}
	g_exit_status = 0;
	if (flag == 1)
		g_exit_status = 1;
	return ;
}
