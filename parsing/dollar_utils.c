/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 21:29:46 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/01 15:35:27 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static int	ft_nbrlen(int nbr)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
		len++;
	while (nbr)
	{
		if (nbr != 0)
		{
			len++;
			nbr /= 10;
		}
	}
	return (len);
}

static char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = ft_nbrlen(n);
	res = malloc((len + 1) * (sizeof(char)));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (n < 0)
		res[0] = '-';
	else if (n == 0)
		res[0] = '0';
	while (n != 0)
	{
		len--;
		res[len] = ft_negorpos(n % 10) + 48;
		n /= 10;
	}
	return (res);
}

void	ft_check_state(int *state, int car)
{
	if (car == '\'')
	{
		if (*state == 1)
			*state = 0;
		else if (*state == 0)
			*state = 1;
	}
	if (car == '\"')
	{
		if (*state == 2)
			*state = 0;
		else if (*state == 0)
			*state = 2;
	}
}

char	*ft_replace_wrongname(char *token, int *flag)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strdup(token);
	tmp[*flag] = 0;
	tmp2 = ft_substr(token, (*flag) + 2, 1000);
	// tmp2 = ft_substr(token, (*flag) + 2, ft_strlen(token), NULL);
	free(token);
	token = ft_strjoin(tmp, tmp2);
	*flag = ft_strlen(tmp);
	free(tmp);
	free(tmp2);
	return (token);
}

char	*ft_replace_exitcode(char *token, int *flag)
{
	char	*tmp;
	char	*tmp2;
	char	*exitcode;

	exitcode = ft_itoa(g_exit_status);
	tmp = ft_substr(token, 0, *flag);
	tmp2 = ft_strjoin(tmp, exitcode);
	free(exitcode);
	free(tmp);
	tmp = ft_substr(token, (*flag) + 2, 1000);
	// tmp = ft_substr(token, (*flag) + 2, ft_strlen(token), NULL);
	free(token);
	token = ft_strjoin(tmp2, tmp);
	*flag = ft_strlen(tmp2);
	free(tmp2);
	free(tmp);
	return (token);
}
