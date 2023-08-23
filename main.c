/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:51:40 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/20 18:22:22 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

void	free_env_list(t_env **head)
{
	t_env	*current;
	t_env	*temp;

	current = *head;
	if (head == NULL || *head == NULL)
		return ;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp->var_name);
		free(temp->value);
		free(temp);
	}
	*head = NULL;
}

char	*read_command(void)
{
	char	*command;

	command = readline(RED "minishell$ " NC);
	if (command && *command)
		add_history(command);
	return (command);
}

void	ft_update_status(char *command)
{
	if (command[0] == '\0' || check_empty_line(command) == 1)
		g_exit_status = 0;
	else
	{
		ft_dprintf(2, "minishell: syntax error\n");
		g_exit_status = 258;
	}
	free (command);
}

void	ft_start(int argc, int *g_exit_status)
{
	g_exit_status = 0;
	if (argc != 1)
	{
		ft_dprintf(2, "minishell: too many arguments\n");
		exit (127);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_token	*token_lst;
	t_env	*env_lst;
	char	*command;

	(void)argv;
	token_lst = NULL;
	ft_start(argc, &g_exit_status);
	env_lst = ft_get_env(env);
	handle_signal(DEFAULT_SIGNAL);
	while (1)
	{
		command = read_command();
		if (command == NULL)
			break ;
		if (*command && parse_line(command, &token_lst) == 1)
			start_exec(&env_lst, command);
		else
		{
			ft_update_status(command);
			continue ;
		}
	}
	rl_clear_history();
	ft_dprintf(STDOUT_FILENO, "exit\n");
	return (g_exit_status);
}
