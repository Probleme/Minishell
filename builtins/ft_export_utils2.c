/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:48:55 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/06 18:06:40 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	**ft_env_value(t_env *env)
{
	char	**env_value;
	int		i;

	i = 0;
	env_value = malloc(sizeof(char *) * (ft_list_size(env) + 1));
	if (!env_value)
	{
		perror("malloc");
		return (NULL);
	}
	while (env != NULL)
	{
		if (env->value)
			env_value[i] = ft_strdup(env->value);
		else
			env_value[i] = NULL;
		env = env->next;
		i++;
	}
	env_value[i] = NULL;
	return (env_value);
}

static void	ft_free_arrays(char **env_name, char **env_value)
{
	int	i;
	int	array_size;

	i = -1;
	array_size = ft_sizeof_array(env_name);
	while (++i <= array_size)
	{
		free(env_name[i]);
		free(env_value[i]);
	}
	free(env_name);
	free(env_value);
}

static void	ft_print_env(char **env_name, char **env_value)
{
	int	i;

	i = -1;
	while (env_name[++i])
	{
		if (!(env_name[i][0] == '_' && ft_strlen(env_name[i]) == 1))
		{
			if (!ft_strcmp(env_name[i], "OLDPWD"))
			{
				dprintf(1, "declare -x OLDPWD\n");
				i++;
			}
			if (env_value[i])
				dprintf(1, "declare -x %s=\"%s\"\n", env_name[i], env_value[i]);
			else
				dprintf(1, "declare -x %s\n", env_name[i]);
		}
	}
}

static void	ft_swap_str(char ***env_name, char ***env_value, int j)
{
	char	*tmp;

	tmp = (*env_name)[j];
	(*env_name)[j] = (*env_name)[j + 1];
	(*env_name)[j + 1] = tmp;
	tmp = (*env_value)[j];
	(*env_value)[j] = (*env_value)[j + 1];
	(*env_value)[j + 1] = tmp;
}

void	ft_print_env_var(t_env *env)
{
	int		i;
	int		j;
	int		lenght_array;
	char	**env_name;
	char	**env_value;

	g_exit_status = 0;
	env_name = ft_env_name(env);
	env_value = ft_env_value(env);
	i = -1;
	lenght_array = ft_sizeof_array(env_name);
	while (++i < lenght_array)
	{
		j = -1;
		while (++j < lenght_array - 1)
			if (ft_strcmp(env_name[j], env_name[j + 1]) > 0)
				ft_swap_str(&env_name, &env_value, j);
	}
	ft_print_env(env_name, env_value);
	ft_free_arrays(env_name, env_value);
}
