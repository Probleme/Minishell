/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 00:40:25 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/06 18:10:09 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	*ft_get_var_name(char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '=')
			break ;
		i++;
	}
	var = malloc(sizeof(char) * i + 1);
	if (!var)
		return (NULL);
	ft_strlcpy(var, str, i + 1);
	return (var);
}

void	ft_export_concatenate(char *str, t_env **env, char *old_value)
{
	char	*var;
	t_env	*tmp;

	var = ft_get_var_name(str);
	tmp = ft_list_search(*env, var);
	free(var);
	if (tmp && tmp->value)
	{
		old_value = ft_strdup(tmp->value);
		free(tmp->value);
		tmp->value = ft_strjoin(old_value, ft_strchr(str, '=') + 1);
		free(old_value);
	}
	else if (tmp)
	{
		ft_list_clearone(env, tmp);
		ft_list_add_back(env, ft_new_list(str));
	}
	else
		ft_list_add_back(env, ft_new_list(str));
}

void	ft_export_assign(char *str, t_env **env)
{
	char	*var;
	t_env	*tmp;

	var = ft_get_var_name(str);
	tmp = ft_list_search(*env, var);
	free(var);
	if (tmp)
	{
		free(tmp->value);
		tmp->value = ft_strdup(ft_strchr(str, '=') + 1);
	}
	else
		ft_list_add_back(env, ft_new_list(str));
}

void	ft_export_create_var(char *str, t_env **env)
{
	char	*var;
	t_env	*tmp;

	var = ft_get_var_name(str);
	tmp = ft_list_search(*env, var);
	free(var);
	if (!tmp)
		ft_list_add_back(env, (ft_new_list(str)));
}
