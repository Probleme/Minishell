/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 15:16:11 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/24 09:49:25 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	*apply_delete(char *token, int *i)
{
	char	*tmp[2];
	char	*new_token;
	char	quote;
	int		j;

	quote = token[*i];
	j = *i + 1;
	while (token[j] && token[j] != quote)
		j++;
	tmp[0] = ft_substr(token, 0, *i);
	tmp[1] = ft_substr(token, *i + 1, j - *i - 1);
	new_token = ft_strjoin(tmp[0], tmp[1]);
	free(tmp[0]);
	free(tmp[1]);
	*i = j - 2;
	tmp[0] = ft_strjoin(new_token, token + j + 1);
	free(token);
	free(new_token);
	token = ft_strdup(tmp[0]);
	free(tmp[0]);
	return (token);
}

static char	*delete_token(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
			token = apply_delete(token, &i);
		i++;
	}
	return (token);
}

void	delete_quotes(char **command, t_exec *exec)
{
	int	i;
	int	j;

	i = 0;
	exec->is_quote = 0;
	while (command[i])
	{
		if (exec->old_token[i] == SIGNAL_HEREDOC)
			i = i + 2;
		if (command[i] == NULL)
			break ;
		j = 0;
		if (((command[i] && ft_strchr(command[i], '\'')) || (command[i]
					&& ft_strchr(command[i], '"'))))
		{
			exec->is_quote = 1;
			command[i] = delete_token(command[i]);
		}
		i++;
	}
}
