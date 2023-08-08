/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 23:50:42 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/08 10:48:11 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

void	close_unused_hd(int heredoc_fd[16][2], int cmd_nb)
{
	int	i;

	i = -1;
	while (++i < 16)
		if (heredoc_fd[i][0] == cmd_nb)
			close(heredoc_fd[i][1]);
}

int	ft_strlcpy(char *dst, char *src, size_t size)
{
	int	cnt;

	cnt = 0;
	while (cnt + 1 < (int)size && src[cnt])
	{
		dst[cnt] = src[cnt];
		cnt++;
	}
	if (size)
		dst[cnt] = 0;
	return (ft_strlen(src));
}

char	*ft_strchr(const char *s, int c)
{
	int		cnt;
	char	*str;

	cnt = 0;
	str = (char *)s;
	while (c > 255)
		c -= 256;
	while (*str)
	{
		if (*str == c)
			return (str + cnt);
		str++;
	}
	if (*str == c)
		return (str);
	return (NULL);
}
