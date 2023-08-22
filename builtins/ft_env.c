/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:31:57 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 13:57:01 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	ft_fill_env(t_env **env, char *str, t_env *tmp)
{
	t_env	*curr;
	char	cwd[4096];

	tmp = ft_new_list(str);
	curr = *env;
	if (ft_strcmp(str, "PWD=") == 0)
	{
		free(tmp->value);
		getcwd(cwd, sizeof(cwd));
		tmp->value = ft_strdup(cwd);
	}
	while (curr != NULL)
	{
		if (strcmp(curr->var_name, tmp->var_name) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(tmp->value);
			free(tmp->var_name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		curr = curr->next;
	}
	ft_list_add_back(env, tmp);
}

static int	ft_is_env(t_env *env, char *var_name)
{
	t_env	*curr;

	curr = env;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->var_name, var_name) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

static char	*ft_get_value(t_env *env, char *var_name)
{
	t_env	*curr;

	curr = env;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->var_name, var_name) == 0)
			return (ft_strdup(curr->value));
		curr = curr->next;
	}
	return (NULL);
}

static void	ft_add_shlvl(t_env *env)
{
	char	*shlvl;
	int		shlvl_value;
	char	*shlvl_new;
	char	*tmp_shlvl;

	if (ft_is_env(env, "SHLVL"))
	{
		shlvl = ft_get_value(env, "SHLVL");
		shlvl_value = ft_atoi(shlvl) + 1;
		if (shlvl_value < 0)
			shlvl_value = 0;
		else if (shlvl_value > 1000)
			shlvl_value = 1;
		free(shlvl);
		tmp_shlvl = ft_itoa(shlvl_value);
		shlvl_new = ft_strjoin("SHLVL=", tmp_shlvl);
		ft_fill_env(&env, shlvl_new, NULL);
		free(shlvl_new);
		free(tmp_shlvl);
	}
}

t_env	*ft_get_env(char **envp)
{
	int		i;
	t_env	*env;
	char	*path;
	char	*tmp;

	i = 0;
	env = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "OLDPWD", 6) != 0)
			ft_list_add_back(&env, ft_new_list(envp[i]));
		i++;
	}
	ft_add_shlvl(env);
	path = ft_strjoin("PATH=", _PATH_STDPATH);
	tmp = ft_strdup(path);
	if (ft_is_env(env, "SHLVL") == 0)
		ft_fill_env(&env, "SHLVL=1", NULL);
	ft_fill_env(&env, "PWD=", NULL);
	ft_fill_env(&env, path, NULL);
	free(path);
	free(tmp);
	return (env);
}
