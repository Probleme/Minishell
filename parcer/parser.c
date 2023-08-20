/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:54:34 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/08/20 16:33:16 by ataouaf          ###   ########.fr       */
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

int	ft_redir_case(t_token *t)
{
	if (ft_strcmp(t->token, "<<") == 0 && ft_strcmp(t->next->token, "<") == 0)
		return (ft_error_msg("minishell: syntax error near unexpected token `newline'\n", 2)
			, 0);
	if (ft_strcmp(t->token, ">>") == 0 && ft_strcmp(t->next->token, ">") == 0)
		return (ft_error_msg("minishell: syntax error near unexpected token `>'\n", 2), 0);
	if (ft_is_a_redirection(t) == 1 && t->next->type == _pipe)
		return (ft_error_msg("minishell: syntax error near unexpected token `|'\n", 2), 0);
	if (ft_is_a_redirection(t) == 1 && ft_is_a_redirection(t->next) == 1)
		return (ft_error_msg("minishell: syntax error near unexpected token `", 2), 
			ft_error_msg(t->next->token, 2), ft_error_msg("\'\n", 2), 0);
	if (ft_is_a_redirection(t) == 1 && t->next == NULL)
		return (ft_error_msg("minishell: syntax error near unexpected token `newline'\n", 2)
			, 0);
	return (1);
}

int	ft_parsing(t_token *t)
{
	if (t && t->type == _pipe) 
		return (ft_error_msg("minishell: syntax error near unexpected token `|'\n", 2), 0);
	while (t)
	{
		if ((t->type == _single_quote || t->type == _double_quote)
			&& ft_check_close(t) == 0)
			return (ft_error_msg("minishell: syntax error\n", 2), 0);
		if (t->type == _pipe && t->next == NULL)
			return (ft_error_msg("minishell: syntax error near unexpected token `|'\n", 2), 0);
		if (t->next != NULL)
		{
			if (ft_redir_case(t) == 0)
				return (0);
			if(t->type == _pipe && t->next->type == _pipe && t->next->next == NULL)
				return (ft_error_msg("minishell: syntax error near unexpected token `|'", 2), 0);
		}
		t = t->next;
	}
	return (1);
}
