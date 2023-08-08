/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:46:13 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 14:13:41 by ataouaf          ###   ########.fr       */
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

static void ft_change_dir(t_env *env, char *dir, char *oldpwd)
{
	t_env *pwdsearch;
	char pwd[4097];
	char *temp;

	if (!chdir(dir))
	{
		getcwd(pwd, 4096);
		pwdsearch = ft_list_search(env, "PWD");
		if (pwdsearch)
			ft_list_clearone(&env, pwdsearch);
		temp = ft_strjoin("PWD=", pwd);
		ft_list_add_back(&env, ft_new_list(temp));
		free(temp);
		set_oldpwd(oldpwd, env);
	}
}

static int	ft_dir_home(t_env *env)
{
	t_env	*beforesearch;
	char	*dir;
	char	*oldpwd;
	t_env	*tmp;

	dir = NULL;
	oldpwd = NULL;
	tmp = ft_list_search(env, "HOME");
	if (tmp && tmp->value)
		dir = ft_strdup(tmp->value);
	beforesearch = ft_list_search(env, "PWD");
	if (beforesearch && beforesearch->value)
		oldpwd = ft_strdup(beforesearch->value);
	ft_change_dir(env, dir, oldpwd);
	return (0);
}

static void	ft_error(char *args, char *old_pwd)
{
	free(old_pwd);
	ft_error_msg("minishell: cd: ", STDERR_FILENO);
	g_exit_status = 1;
	perror(args);
}

void	ft_cd(char **str, t_env *env, t_exec *exec)
{
	(void)exec;
	char	pwd[4097];
	char	*oldpwd;
	t_env	*pwdsearch;
	t_env	*beforesearch;
	char	*tmp;

	if (!ft_strcmp("cd", str[0]) && !str[1])
	{
		ft_dir_home(env);
		return;
	}
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
