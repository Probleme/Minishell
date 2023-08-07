/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hed-dyb <hed-dyb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:49:42 by hed-dyb           #+#    #+#             */
/*   Updated: 2023/07/10 18:20:02 by hed-dyb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_space_or_pipe(t_token **t, char *command, int i)
{
	t_token	*node;

	node = NULL;
	if (ft_is_a_white_space(command[i]) == 1)
	{
		node = ft_create_node(ft_strdup(" "), _white_space, *t);
		ft_add_back(t, node);
		while (command[i])
		{
			if (command[i + 1] == '\0' || ft_is_a_white_space(command[i + 1])
				== 0)
				break ;
			i++;
		}
		return (i);
	}
	if (command[i] == '|')
	{
		node = ft_create_node(ft_strdup("|"), _pipe, *t);
		ft_add_back(t, node);
		return (i);
	}
	return (i);
}

int	ft_redirections(t_token **t, char *command, int i)
{
	t_token	*node;

	node = NULL;
	if (command[i] == '>' && command[i + 1] == '>')
	{
		node = ft_create_node(ft_strdup(">>"), _append_output_re, *t);
		i++;
	}
	else if (command[i] == '<' && command[i + 1] == '<')
	{
		node = ft_create_node(ft_strdup("<<"), _here_document, *t);
		i++;
	}
	else if (command[i] == '>')
		node = ft_create_node(ft_strdup(">"), _output_re, *t);
	else if (command[i] == '<')
		node = ft_create_node(ft_strdup("<"), _input_re, *t);
	ft_add_back(t, node);
	return (i);
}

int	ft_double_qoute(t_token **t, char *command, int i)
{
	int		start;
	char	*tok;
	int		j;
	t_token	*node;

	j = 0;
	start = i;
	i = ft_char_indice(command, i, '"');
	tok = malloc((i - start + 2) * sizeof(char));
	ft_protection(tok, command, t);
	while (command[start])
	{
		tok[j] = command[start];
		if (start == i)
			break ;
		j++;
		start++;
	}
	tok[j + 1] = '\0';
	node = ft_create_node(tok, _double_quote, *t);
	ft_add_back(t, node);
	return (i);
}

int	ft_single_quote(t_token **t, char *command, int i)
{
	t_token	*node;
	int		start;
	char	*tok;
	int		j;

	j = 0;
	start = i;
	i = ft_char_indice(command, i, '\'');
	tok = malloc((i - start + 2) * sizeof(char));
	ft_protection(tok, command, t);
	while (command[start])
	{
		tok[j] = command[start];
		if (start == i)
			break ;
		j++;
		start++;
	}
	tok[j + 1] = '\0';
	node = ft_create_node(tok, _single_quote, *t);
	ft_add_back(t, node);
	return (i);
}

void	ft_create_tokens(t_token **t, char *command)
{
	int	i;

	i = 0;
	while (command[i] && ft_is_a_white_space(command[i]) == 1)
		i++;
	while (command && command[i])
	{
		if (ft_is_a_white_space(command[i]) == 1 || command[i] == '|')
			i = ft_space_or_pipe(t, command, i);
		else if (command[i] == '<' || command[i] == '>')
			i = ft_redirections(t, command, i);
		else if (command[i] == '\"')
			i = ft_double_qoute(t, command, i);
		else if (command[i] == '\'')
			i = ft_single_quote(t, command, i);
		else
			i = ft_is_a_word(t, command, i);
		i++;
	}
	ft_unnecessary_spaces(t);
}
