/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:46:13 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:33:21 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	set_oldpwd(char *oldpwd, t_env *env)
{
	char	*tmp;
	t_env	*pwdsearch;

	pwdsearch = ft_list_search(env, "OLDPWD");
	if (pwdsearch)
		ft_list_clearone(&env, pwdsearch);
	if (oldpwd)
	{
		tmp = ft_strjoin("OLDPWD=", oldpwd);
		free(oldpwd);
		ft_list_add_back(&env, ft_new_list(tmp));
		free(tmp);
	}
	else
		ft_list_add_back(&env, ft_new_list("OLDPWD"));
}

static int	ft_check_dir_home(char **args, t_exec *exec)
{
	t_env	*pwdsearch;
	char	pwd[4096];
	t_env	*beforesearch;
	char	*tempo;
	char	*dir;
	char	*oldpwd;
	t_env	*tmp;

	oldpwd = NULL;
	if (args[1] == NULL)
	{
		tmp = ft_list_search(*exec->env, "HOME");
		if (tmp && tmp->value)
			dir = ft_strdup(tmp->value);
		beforesearch = ft_list_search(*exec->env, "PWD");
		if (beforesearch && beforesearch->value)
			oldpwd = ft_strdup(beforesearch->value);
		if (!chdir(dir))
		{
			getcwd(pwd, 4096);
			pwdsearch = ft_list_search(*exec->env, "PWD");
			if (pwdsearch)
				ft_list_clearone(exec->env, pwdsearch);
			tempo = ft_strjoin("PWD=", pwd);
			ft_list_add_back(exec->env, ft_new_list(tempo));
			free(tempo);
			set_oldpwd(oldpwd, *exec->env);
			return (0);
		}
	}
	return (1);
}

static void	ft_error(char *args, char *old_pwd)
{
	g_exit_status = 1;
	free(old_pwd);
	dprintf(2, "minishell: cd: ");
	perror(args);
}

void	ft_cd(char **str, t_env *env, t_exec *exec)
{
	char	pwd[4097];
	char	*oldpwd;
	t_env	*pwdsearch;
	t_env	*beforesearch;
	char	*tmp;

	if (!ft_check_dir_home(str, exec))
		return ;
	beforesearch = ft_list_search(env, "PWD");
	oldpwd = NULL;
	if (beforesearch && beforesearch->value)
		oldpwd = ft_strdup(beforesearch->value);
	if (!chdir(str[1]))
	{
		getcwd(pwd, 4096);
		pwdsearch = ft_list_search(env, "PWD");
		if (pwdsearch)
			ft_list_clearone(&env, pwdsearch);
		tmp = ft_strjoin("PWD=", pwd);
		ft_list_add_back(&env, ft_new_list(tmp));
		free(tmp);
		set_oldpwd(oldpwd, env);
	}
	else
		ft_error(str[1], oldpwd);
}
