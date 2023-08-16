/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:46:13 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/16 18:05:23 by ataouaf          ###   ########.fr       */
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

static void ft_change_dir(t_env *env, char *dir, char *oldpwd)
{
	t_env *pwdsearch;
	char pwd[4096];
	char *temp;

	if (!chdir(dir))
	{
		getcwd(pwd, 4096);
		pwdsearch = ft_list_search(env, "PWD");
		if (pwdsearch)
			ft_list_clearone(&env, pwdsearch);
		temp = ft_strjoin("PWD=", pwd);
		temp = ft_strdup(temp);
		ft_list_add_back(&env, ft_new_list(temp));
		free(temp);
		set_oldpwd(oldpwd, env);
	}
	free(dir);
	
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
	ft_dprintf(STDERR_FILENO, "minishell: cd: ");
	g_exit_status = 1;
	perror(args);
}

void	ft_cd(char **str, t_env *env, t_exec *exec)
{
	(void)exec;
	char	pwd[4096];
	char	*oldpwd;
	t_env	*pwdsearch;
	t_env	*pwd_node;
	t_env	*beforesearch;
	char	*tmp;

	if (!ft_strcmp("cd", str[0]) && !str[1])
	{
		if (!ft_list_search(env, "HOME"))
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			g_exit_status = 1;
			return ;
		}
		ft_dir_home(env);
		return;
	}
	if (!ft_list_search(env, "PWD"))
	{
		pwd_node = malloc(sizeof(t_env));
		pwd_node->var_name = ft_strdup("PWD");
		pwd_node->value = ft_strdup(getcwd(pwd, 4096));
		pwd_node->next = NULL;
		ft_list_add_back(&env, pwd_node);
	}
	beforesearch = ft_list_search(env, "PWD");
	oldpwd = NULL;
	if (beforesearch && beforesearch->value)
		oldpwd = ft_strdup(beforesearch->value);
	if (!chdir(str[1]))
	{
		if (getcwd(pwd, sizeof(pwd)) == NULL)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			g_exit_status = 1;
			return ;	
		}
		
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
