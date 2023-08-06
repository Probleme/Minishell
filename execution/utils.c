/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:42:37 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/05 13:08:01 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;
	size_t	i;

	i = 0;
	res = malloc(count * size);
	if (!res)
		return (0);
	while (i < (count * size))
	{
		((char *)res)[i] = 0;
		i++;
	}
	return (res);
}

int	count_cmd(char **command, int *tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (command[i] && tokens[i])
	{
		if (tokens[i] == PIPE)
			count++;
		i++;
	}
	return (count + 1);
}

int	ft_sizeof_array(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

void	close_all_fd(int pipe_fd[2][2], int cmd_nb)
{
	close_fd(pipe_fd[(cmd_nb + 1) % 2][0]);
	close_fd(pipe_fd[cmd_nb % 2][0]);
	close_fd(pipe_fd[cmd_nb % 2][1]);
}

void	ft_free_arr(void **arr)
{
	int	i;

	i = -1;
	while (arr && arr[++i])
		free(arr[i]);
	free(arr);
}
