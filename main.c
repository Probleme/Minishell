/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:51:40 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/10 13:11:06 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

int		g_exit_status = 0;

void free_env_list(t_env **head)
{
    t_env *current = *head;
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

int main(int argc, char **argv, char **env)
{
    t_token *token_lst = NULL;
    t_env *env_lst;
    char *command;
    (void)argv;

    g_exit_status = 0;
    if (argc != 1)
        exit(0);
    env_lst = ft_get_env(env);
    handle_signal(DEFAULT_SIGNAL);
    while ((command = read_command()))
        if (*command && parse_line(command, &token_lst) == 1)
            start_exec(&env_lst, command);
		else
			free(command);
    rl_clear_history();
    free_env_list(&env_lst);
    ft_error_msg("exit\n", STDOUT_FILENO);
    return g_exit_status;
}
