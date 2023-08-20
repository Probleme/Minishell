/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 20:21:36 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 16:29:09 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	*apply_expansion_bis(char *token, int *i, t_env *env)
{
	if ((token[*i] == '$' && ft_isalpha(token[*i + 1])) || (token[*i] == '$'
			&& token[*i + 1] == '_'))
	{
		token = ft_var_replace(token, i, env);
		if (token[*i] == '$' || token[*i] == '"' || token[*i] == '\'')
			*i -= 1;
		else if (token[*i] == 0)
			*i = -2;
	}
	else if (token[*i] == '$')
	{
		token = ft_replace_wrongname(token, i);
		if (token[*i] == '$' || token[*i] == '"' || token[*i] == '\'')
			*i -= 1;
		else if (token[*i] == 0)
			*i = -2;
	}
	return (token);
}

static char	*apply_expansion(char *token, t_env *env)
{
	int	i;

	i = 0;
	while (i != -1 && token[i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			token = ft_replace_exitcode(token, &i);
			if (token[i] == '$' || token[i] == '"' || token[i] == '\'')
				continue ;
			else if (token[i] == 0)
				break ;
		}
		else if (token[i] == '$' && token[i + 1] == '$' && ft_isalpha(token[i
				+ 2]))
				return (token);
		else if (token[i] == '$' && (token[i + 1] == ' ' || token[i + 1] == '\''
				|| token[i + 1] == '"' || token[i + 1] == '$' || !token[i + 1]
				|| token[i + 1] == '/'))
			;
		else
			token = apply_expansion_bis(token, &i, env);
		i++;
	}
	return (token);
}

void	ft_expand(char **command, t_env *env)
{
	int	i;

	i = -1;
	while (command[++i])
	{
		if (ft_strchr(command[i], '$'))
			command[i] = apply_expansion(command[i], env);
	}
}
