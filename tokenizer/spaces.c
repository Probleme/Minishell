/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 15:38:04 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/20 16:27:04 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_special_char(t_token *node)
{
	if (node == NULL)
		return (0);
	if (node->type >= 1 && node->type <= 5)
		return (1);
	return (0);
}

void	ft_remove_space(t_token *temp)
{
	t_token	*after;
	t_token	*befor;

	if (temp->type == _white_space)
	{
		if (ft_special_char(temp->previous) && ft_special_char(temp->next))
		{
			after = temp->next;
			befor = temp->previous;
			free(temp->token);
			free(temp);
			befor->next = after;
			after->previous = befor;
		}
	}
}

void	ft_unnecessary_spaces(t_token **t)
{
	t_token	*temp;

	temp = *t;
	while (temp)
	{
		ft_remove_space(temp);
		break ; // problem here using break to avoid read a freed memory
		temp = temp->next;
	}
	temp = *t;
	while (temp && temp->next)
	{
		if (temp->next->type == _white_space && temp->next->next == NULL)
		{
			free(temp->next->token);
			free(temp->next);
			temp->next = NULL;
			break ;
		}
		temp = temp->next;
	}
}
