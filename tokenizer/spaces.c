/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hed-dyb <hed-dyb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 15:38:04 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/07/10 19:14:12 by hed-dyb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_special_char (t_token *node)
{
	if(node == NULL)
		return 0;
	if(node->type >= 1 && node->type <= 5)
		return (1);
	return 0;
}

void ft_unnecessary_spaces(t_token **t)
{
	t_token *temp;
	t_token *after;
	t_token *befor;

	temp = *t;
	while(temp)
	{
		if(temp->type == _white_space)
		{
			if(ft_special_char(temp->previous) == 1 && ft_special_char(temp->next) == 1)
			{
				after = temp->next;
				befor = temp->previous;
				befor->next = after;
				after->previous = befor;
			}
		}
		temp = temp->next;
	}
	temp = *t;
	while(temp && temp->next)
	{
		if(temp->next->next == NULL && temp->next->type == _white_space)
		{
			temp->next = NULL;
			break ;
		}
		temp = temp->next;
	}
}
