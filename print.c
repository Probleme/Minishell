/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 17:34:20 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/07/20 19:34:23 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_print_t_token_linked_list(t_token *t)//
{
	if(t == NULL)
		printf("NULL\n");
		int i = 0;
	while(t)
	{
		printf("%d ==> %s  %s!\n", i, t->token, typeNames[t->type]);
		i++;
		t = t->next;
	}
	printf("\n\n");
}

void ft_print_t_env_linked_list(t_env *e)//
{
	if(e == NULL)
		printf("NULL\n");
		int i = 0;
	while(e)
	{
		printf("%d ==> %s      %s\n", i, e->var_name, e->value);
		i++;
		e = e->next;
	}
	printf("\n\n");
}

void ft_printf_double_ptr(char **str)
{
	if(str == NULL)
	{
		printf("NULL\n");
		return ;
	}
	int i = 0;
	while(str[i])
	{
		printf("%s  - ", str[i]);
		i++;
	}
	printf("\n");
}

void ft_print_t_cmd_linked_list(t_cmd *cmd)
{
	if(cmd == NULL)
	{
		printf("NULL\n");
		return ;
	}
	while(cmd)
	{
		int j = 0;
		if(cmd->comand == NULL)
			printf(" command NULL\n");
		else
		{
			while(cmd->comand[j])
			{
				printf("command[%d]  %s\n",j, cmd->comand[j]);
				j++;
			}
		}

		printf("\n");
		cmd = cmd->next;
	}
}


