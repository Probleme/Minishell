/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 20:21:36 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:35:30 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	ft_free_this(char *tmp, char *tmp2, char *var_name)
{
	free(tmp);
	free(tmp2);
	free(var_name);
}

static char	*ft_var_replace(char *token, int *i, t_env *env)
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
	ft_free_this(tmp, tmp2, var_name);
	return (token);
}

static char	*apply_expansion_bis(char *token, int *i, int state, t_env *env)
{
	if ((token[*i] == '$' && state != 1 && ft_isalpha(token[*i + 1]))
		|| (token[*i] == '$' && state != 1 && token[*i + 1] == '_'))
	{
		token = ft_var_replace(token, i, env);
		if (token[*i] == '$' || token[*i] == '"' || token[*i] == '\'')
			*i -= 1;
		else if (token[*i] == 0)
			*i = -2;
	}
	else if (token[*i] == '$' && state != 1)
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
	int	state;

	i = 0;
	state = 0;
	while (i != -1 && token[i])
	{
		ft_check_state(&state, token[i]);
		if (token[i] == '$' && state != 1 && token[i + 1] == '?')
		{
			token = ft_replace_exitcode(token, &i);
			if (token[i] == '$' || token[i] == '"' || token[i] == '\'')
				continue ;
			else if (token[i] == 0)
				break ;
		}
		else if (token[i] == '$' && state != 1 && (token[i + 1] == ' '
				|| token[i + 1] == '\'' || token[i + 1] == '"' || token[i
				+ 1] == '$' || !token[i + 1] || token[i + 1] == '/'))
			;
		else
			token = apply_expansion_bis(token, &i, state, env);
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
		if (ft_strchr(command[i], '$'))
			command[i] = apply_expansion(command[i], env);
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
