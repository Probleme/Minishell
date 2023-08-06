/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:21:45 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:33:18 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

int	ft_list_size(t_env *env)
{
	int	i;

	i = 0;
	while (env != NULL)
	{
		env = env->next;
		i++;
	}
	return (i);
}

t_env	*ft_list_search(t_env *env, char *search)
{
	t_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->var_name, search))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void	ft_delnode(t_env *env)
{
	if (!env)
		return ;
	free(env->var_name);
	free(env->value);
	free(env);
}

void	ft_list_clearone(t_env **root, t_env *todel)
{
	t_env	*current;

	if (!root || !*root || !todel)
		return ;
	if (*root == todel && !(*root)->next)
	{
		ft_delnode(todel);
		*root = NULL;
		return ;
	}
	else if (*root == todel)
	{
		*root = (*root)->next;
		ft_delnode(todel);
		return ;
	}
	current = *root;
	while (current->next != NULL && current->next != todel)
		current = current->next;
	if (current->next == todel)
	{
		current->next = todel->next;
		ft_delnode(todel);
	}
}
