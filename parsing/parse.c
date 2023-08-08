/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:41:20 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 11:20:09 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../exec.h"

void	ft_free_token(t_token *t)
{
	t_token	*last;

	if (t == NULL)
		return ;
	while (t)
	{
		last = t;
		t = t->next;
		free (last->token);
		free(last);
	}
}

void	ft_error_msg(char *msg, int fd)
{
	int	i;

	i = -1;
	while (msg[++i])
		write(fd, &msg[i], 1);
	g_exit_status = 2;
}

void	ft_protection(char *to_check, void *optional, t_token **t)
{
	if (to_check == NULL)
	{
		if (optional != NULL)
			free (optional);
		ft_free_token(*t);
		exit (1);
	}
}

int	parse_line(char *command, t_token **token_lst)
{
	ft_create_tokens(token_lst, command);
	if (!ft_parsing(*token_lst))
	{
		ft_free_token(*token_lst);
		*token_lst = NULL;
		return (0);
	}
	ft_free_token(*token_lst);
	*token_lst = NULL;
	return (1);
}
