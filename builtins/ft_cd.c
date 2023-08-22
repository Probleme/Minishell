/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:46:13 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 13:44:20 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	set_oldpwd(char *oldpwd, t_env *env)
{
	char	*tmp;
	t_env	*pwdsearch;

	pwdsearch = ft_list_search(env, "OLDPWD");
	if (pwdsearch)
	{
		ft_list_clearone(&env, pwdsearch);
		pwdsearch = NULL;
	}
	free(pwdsearch);
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

static void	ft_change_dir(t_env *env, char *dir, char *oldpwd)
{
	if (!chdir(dir))
	{
		set_pwd(env, dir);
		set_oldpwd(oldpwd, env);
	}
	else
		ft_error_cd(dir, oldpwd);
}

static int	ft_dir_home(t_env *env, char **str, char *dir)
{
	t_env	*beforesearch;
	char	*oldpwd;
	t_env	*home;

	if (!str[1])
	{
		home = ft_list_search(env, "HOME");
		if (!home || !home->value)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			g_exit_status = 1;
			return (1);
		}
		else
			dir = ft_strdup(home->value);
		oldpwd = NULL;
		beforesearch = ft_list_search(env, "PWD");
		if (beforesearch && beforesearch->value)
			oldpwd = ft_strdup(beforesearch->value);
		ft_change_dir(env, dir, oldpwd);
		free(dir);
		return (1);
	}
	return (0);
}

static void	handle_change_to_special_dirs(t_env *env, char **str, char *oldpwd)
{
	char	*tmp;
	char	*new_path;

	new_path = NULL;
	tmp = ft_strjoin(oldpwd, "/");
	new_path = ft_strjoin(tmp, str[1]);
	free(tmp);
	set_pwd(env, new_path);
	set_oldpwd(oldpwd, env);
	free(new_path);
}

void	ft_cd(char **str, t_env *env, char *oldpwd)
{
	t_env	*pwdsearch;
	char	pwd[4096];

	if (ft_dir_home(env, str, NULL))
		return ;
	pwdsearch = ft_list_search(env, "PWD");
	if (pwdsearch && pwdsearch->value)
		oldpwd = ft_strdup(pwdsearch->value);
	if (!chdir(str[1]))
	{
		if (getcwd(pwd, sizeof(pwd)))
		{
			set_pwd(env, pwd);
			set_oldpwd(oldpwd, env);
		}
		else
		{
			if (!ft_strcmp(str[1], ".") || !ft_strcmp(str[1], ".."))
				handle_change_to_special_dirs(env, str, oldpwd);
			else
				ft_error_cd(str[1], oldpwd);
		}
	}
	else
		ft_error_cd(str[1], oldpwd);
}
