/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:54:34 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/20 17:32:33 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../exec.h"

int	ft_is_wrd_qot(t_token *temp)
{
	if (temp == NULL)
		return (0);
	if (temp->type == _word)
		return (1);
	if (temp->type == _double_quote)
		return (1);
	if (temp->type == _single_quote)
		return (1);
	return (0);
}

int	ft_is_a_redirection(t_token *node)
{
	if (node->type == _output_re)
		return (1);
	else if (node->type == _input_re)
		return (1);
	else if (node->type == _append_output_re)
		return (1);
	else if (node->type == _here_document)
		return (1);
	else
		return (0);
}

int	ft_check_close(t_token *t)
{
	int	len;

	len = ft_strlen(t->token);
	if (ft_strlen(t->token) == 1)
		return (0);
	if (t->token[0] == '\'' && t->token[len - 1] == '\'')
		return (1);
	if (t->token[0] == '\"' && t->token[len - 1] == '\"')
		return (1);
	return (0);
}

int	ft_redir_case(t_token *tok)
{
	if (ft_strcmp(tok->token, "<<") == 0
		&& ft_strcmp(tok->next->token, "<") == 0)
		return (0);
	if (ft_strcmp
		(tok->token, ">>") == 0 && ft_strcmp(tok->next->token, ">") == 0)
		return (0);
	if (ft_is_a_redirection(tok) == 1 && tok->next->type == _pipe)
		return (0);
	if (ft_is_a_redirection(tok) == 1 && ft_is_a_redirection(tok->next) == 1)
		return (0);
	return (1);
}

int	ft_parsing(t_token *tok)
{
	if (tok && tok->type == _pipe) 
		return (0);
	while (tok)
	{
		if ((tok->type == _single_quote || tok->type == _double_quote)
			&& ft_check_close(tok) == 0)
			return (0);
		if (tok->type == _pipe && tok->next == NULL)
			return (0);
		if (ft_is_a_redirection(tok) == 1 && tok->next == NULL)
			return (0);
		if (tok->next != NULL)
		{
			if (ft_redir_case(tok) == 0)
				return (0);
		}
		tok = tok->next;
	}
	return (1);
}
