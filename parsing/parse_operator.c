/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:27:05 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:35:35 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static char	*ft_add_space_after_operator(char *str, int i)
{
	char	*new_str;
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(str, 0, i + 1);
	tmp2 = ft_strjoin(tmp, " ");
	free(tmp);
	tmp = ft_substr(str, i + 1, 1000);
	// tmp = ft_substr(str, i + 1, ft_strlen(str) - i, NULL);
	new_str = ft_strjoin(tmp2, tmp);
	free(tmp);
	free(tmp2);
	free(str);
	return (new_str);
}

static char	*ft_check_after_operator(char *new_str, int i, int *lenght)
{
	if ((new_str[i + 1] && !ft_is_space(new_str[i + 1])
			&& !ft_is_redirection(new_str[i + 1])) || *lenght == 2)
	{
		new_str = ft_add_space_after_operator(new_str, i);
		*lenght = 0;
	}
	else
		*lenght = 0;
	return (new_str);
}

static char	*ft_parse_after_operator(char *str)
{
	char	*new_str;
	int		i;
	int		flag;
	int		lenght;

	i = -1;
	lenght = 0;
	flag = 0;
	new_str = ft_strdup(str);
	free(str);
	while (new_str[++i])
	{
		ft_check_state(&flag, new_str[i]);
		if (flag == 0 && (ft_is_redirection(new_str[i])
				|| ft_is_pipe(new_str[i])))
		{
			if (ft_is_redirection(new_str[i]))
				lenght += 1;
			new_str = ft_check_after_operator(new_str, i, &lenght);
		}
	}
	return (new_str);
}

static char	*ft_add_space_before_operator(char *str, int i)
{
	char	*new_str;
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(str, 0, i);
	tmp2 = ft_strjoin(tmp, " ");
	free(tmp);
	// tmp = ft_substr(str, i, ft_strlen(str) - i, NULL);
	tmp = ft_substr(str, i, 1000);
	new_str = ft_strjoin(tmp2, tmp);
	free(tmp);
	free(tmp2);
	free(str);
	return (new_str);
}

char	*ft_operator_parse(char *command)
{
	int		i;
	int		flag;
	char	*str;

	i = -1;
	flag = 0;
	str = ft_strdup(command);
	free(command);
	while (str[++i])
	{
		ft_check_state(&flag, str[i]);
		if (flag == 0 && (ft_is_redirection(str[i]) || ft_is_pipe(str[i])))
		{
			if (i > 1 && ft_strlen(str) > 1 && !ft_is_space(str[i - 1])
				&& !ft_is_redirection(str[i - 1]))
				str = ft_add_space_before_operator(str, i);
		}
	}
	str = ft_parse_after_operator(str);
	return (str);
}
