/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 20:21:36 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/23 20:43:40 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	ft_free(char *tmp, char *tmp2, char *var_name)
{
	free(tmp);
	free(tmp2);
	free(var_name);
}

char	*ft_var_replace(char *token, int *i, t_env *env)
{
	t_env	*env_p;
	char	*tmp;
	char	*tmp2;
	char	*var_name;
	int		end_var;

	end_var = (*i) + 1;
	while ((token[end_var] && ft_isalnum(token[end_var])) || (token[end_var]
			&& token[end_var] == '_'))
		end_var++;
	var_name = ft_substr(token, (*i) + 1, end_var - (*i) - 1);
	env_p = ft_list_search(env, var_name);
	tmp = ft_substr(token, 0, *i);
	if (env_p && env_p->value)
		tmp2 = ft_strjoin(tmp, env_p->value);
	else
		tmp2 = ft_strjoin(tmp, "");
	free(tmp);
	tmp = ft_substr(token, end_var, 1000);
	free(token);
	token = ft_strjoin(tmp2, tmp);
	*i = ft_strlen(tmp2);
	ft_free(tmp, tmp2, var_name);
	return (token);
}

static char	*apply_expansion_bis(char *token, int *i, int quote, t_env *env)
{
	if ((token[*i] == '$' && quote != 1 && ft_isalpha(token[*i + 1]))
		|| (token[*i] == '$' && quote != 1 && token[*i + 1] == '_'))
	{
		token = ft_var_replace(token, i, env);
		if (token[*i] == '$' || token[*i] == '"' || token[*i] == '\'')
			*i = *i - 1;
		else if (token[*i] == '\0')
			*i = -2;
	}
	else if (token[*i] == '$' && quote != 1)
	{
		token = ft_replace_wrongname(token, i);
		if (token[*i] == '$' || token[*i] == '"' || token[*i] == '\'')
			*i = *i - 1;
		else if (token[*i] == '\0')
			*i = -2;
	}
	return (token);
}

static char	*apply_expansion(char *token, t_env *env, int quote)
{
	int	i;

	i = 0;
	while (i != -1 && token[i])
	{
		ft_check_state(&quote, token[i]);
		if (token[i] == '$' && quote != 1 && token[i + 1] == '?')
		{
			token = ft_replace_exitcode(token, &i);
			if (token[i] == '\0')
				break ;
		}
		else if (token[i] == '$' && token[i + 1] == '$' && ft_isalpha(token[i
					+ 2]))
			return (token);
		else if (token[i] == '$' && quote != 1 && (token[i + 1] == ' '
				|| token[i + 1] == '\'' || token[i + 1] == '"' || token[i
					+ 1] == '$' || !token[i + 1] || token[i + 1] == '/'))
				;
		else
			token = apply_expansion_bis(token, &i, quote, env);
		i++;
	}
	return (token);
}

void	handle_dollar(char **command, int *tokens, t_env *env)
{
	int	i;
	int	j;

	i = 0;
	while (command[i])
	{
		if (tokens[i] == SIGNAL_HEREDOC)
			i = i + 2;
		if (command[i] == NULL)
			break ;
		if (ft_strchr(command[i], '$'))
			command[i] = apply_expansion(command[i], env, 0);
		j = 0;
		while (command[i][j])
			if (command[i][j] != ' ' || !(command[i][j] >= 9
					&& command[i][j] <= 13))
				break ;
		if (!command[i][j])
			tokens[i] = EMPTY;
		i++;
	}
}
