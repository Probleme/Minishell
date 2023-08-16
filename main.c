/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:51:40 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/16 17:39:14 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

int		g_exit_status = 0;

void free_env_list(t_env **head)
{
    t_env *current = *head;

    if (head == NULL || *head == NULL)
        return ;
    while (current != NULL)
	{
        t_env *temp = current;
        current = current->next;
        free(temp->var_name);
        free(temp->value);
        free(temp);
    }
	*head = NULL;
}

char *read_command()
{
    char *command = readline(RED "minishell$ " NC);
    if (command && *command)
        add_history(command);
    return command;
}

void    ft_get_path_env(t_env **env)
{
    char *path_value;
    t_env *path_node;

    path_value = _PATH_STDPATH;
    path_node = malloc(sizeof(t_env));
    if (!path_node)
        return;
    path_node->var_name = ft_strdup("PATH");
    path_node->value = ft_strdup(path_value);
    path_node->next = NULL;
    *env = path_node;
}

void ft_get_env_null(t_env **env)
{
    char *path_value = _PATH_STDPATH;
    char cwd[4096];
    t_env *path_node;
    t_env *pwd_node;
    t_env *underscore_node;
    t_env  *shlvl_node;

    path_node = malloc(sizeof(t_env));
    if (!path_node)
        return;
    path_node->var_name = ft_strdup("PATH");
    path_node->value = ft_strdup(path_value);
    path_node->next = NULL;
    pwd_node = malloc(sizeof(t_env));
    if (!pwd_node)
    {
        free(path_node->var_name);
        free(path_node->value);
        free(path_node);
        return;
    }
    pwd_node->var_name = ft_strdup("PWD");
    getcwd(cwd, sizeof(cwd));
    pwd_node->value = ft_strdup(cwd);
    pwd_node->next = NULL;
    underscore_node = malloc(sizeof(t_env));
    if (!underscore_node)
    {
        free(path_node->var_name);
        free(path_node->value);
        free(path_node);
        free(pwd_node->var_name);
        free(pwd_node->value);
        free(pwd_node);
        return;
    }
    underscore_node->var_name = ft_strdup("_");
    underscore_node->value = ft_strdup("/usr/bin/env");
    underscore_node->next = NULL;
    shlvl_node = malloc(sizeof(t_env));
    if (!shlvl_node)
    {
        free(path_node->var_name);
        free(path_node->value);
        free(path_node);
        free(pwd_node->var_name);
        free(pwd_node->value);
        free(pwd_node);
        if(underscore_node->value != NULL)
        {
            free(underscore_node->var_name);
            free(underscore_node->value);
        }
        free(underscore_node);
        return;
    }
    shlvl_node->var_name = ft_strdup("SHLVL");
    shlvl_node->value = ft_strdup("1");
    shlvl_node->next = NULL;
    *env = path_node;
    path_node->next = pwd_node;
    pwd_node->next = underscore_node;
    underscore_node->next = shlvl_node;
}


int main(int argc, char **argv, char **env)
{
    t_token *token_lst;
    t_env *env_lst;
    char *command;
    (void)argv;

    token_lst = NULL;
    g_exit_status = 0;
    if (argc != 1)
        exit(0);
    env_lst = ft_get_env(env);
    if (!ft_list_search(env_lst, "PWD") && !ft_list_search(env_lst, "PATH"))
        ft_get_env_null(&env_lst);
    handle_signal(DEFAULT_SIGNAL);
    while ((command = read_command()))
    {
        if (*command && parse_line(command, &token_lst) == 1)
            start_exec(&env_lst, command);
		else
			free(command);
    }
    rl_clear_history();
    free_env_list(&env_lst);
    ft_dprintf(STDOUT_FILENO, "exit\n");
    return g_exit_status;
}
