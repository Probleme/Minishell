/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 23:50:42 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/20 14:04:06 by ataouaf          ###   ########.fr       */
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

	if (!s)
		return (NULL);
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

void	ft_error_cd(char *args, char *old_pwd)
{
	ft_dprintf(STDERR_FILENO, "minishell: cd: ");
	g_exit_status = 1;
	perror(args);
	free(old_pwd);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ps1;
	unsigned char	*ps2;

	i = 0;
	if (n == 0)
		return (0);
	ps1 = (unsigned char *)s1;
	ps2 = (unsigned char *)s2;
	while (i < n - 1 && ps1[i] == ps2[i] && ps1[i] != '\0' && ps2[i] != '\0')
	{
		i++;
	}
	return (ps1[i] - ps2[i]);
}
