/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 13:48:09 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 18:11:36 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

t_env	*ft_list_last(t_env *env)
{
	t_env	*last;

	last = env;
	if (env == NULL)
		return (NULL);
	while (env != NULL)
	{
		last = env;
		env = env->next;
	}
	return (last);
}

void	ft_list_add_back(t_env **env, t_env *new_env)
{
	t_env	*last;

	if (!new_env || !env)
		return ;
	if (*env == NULL)
		*env = new_env;
	else
	{
		last = ft_list_last(*env);
		last->next = new_env;
		new_env->prev = last;
	}
}

static void	create_and_assign(char *str, t_env **new)
{
	int	i;
	int	var_value_size;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			break ;
	}
	if (str[i - 1] == '+')
	{
		(*new)->var_name = ft_calloc(sizeof(char), i);
		ft_strlcpy((*new)->var_name, str, i);
	}
	else
	{
		(*new)->var_name = ft_calloc(sizeof(char), i + 1);
		ft_strlcpy((*new)->var_name, str, i + 1);
	}
	var_value_size = ft_strlen(str) - ft_strlen((*new)->var_name);
	(*new)->value = ft_calloc(sizeof(char), var_value_size);
	ft_strlcpy((*new)->value, str + i + 1, var_value_size);
}

t_env	*ft_new_list(char *str)
{
	t_env	*tmp;

	tmp = ft_calloc(sizeof(t_env), 1);
	if (!tmp)
		return (NULL);
	if (!ft_strchr(str, '='))
	{
		tmp->var_name = ft_strdup(str);
		if (!tmp->var_name)
		{
			free(tmp);
			return (NULL);
		}
		tmp->value = NULL;
	}
	else
		create_and_assign(str, &tmp);
	return (tmp);
}

void	ft_print_err_env(void)
{
	ft_dprintf(STDERR_FILENO, "env: No such file or directory\n");
	g_exit_status = 127;
}
