/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 09:44:32 by ataouaf           #+#    #+#             */
/*   Updated: 2023/08/16 15:11:52 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec.h"

static void	default_signal(int signal)
{
	(void)signal;
	g_exit_status = 1;
	ft_dprintf(STDOUT_FILENO, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

static void	signal_heredoc(int signal)
{
	(void)signal;
	close(0);
}

static void	parent_signal(int signal)
{
	if (signal == SIGINT)
		ft_dprintf(STDOUT_FILENO, "\n");
	else if (signal == SIGQUIT)
	{
		ft_dprintf(STDERR_FILENO, "Quit: 3\n");
		g_exit_status = 131;
	}
}

void	handle_signal(int sig)
{
	rl_catch_signals = 0;
	if (sig == DEFAULT_SIGNAL)
	{
		signal(SIGINT, default_signal);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig == HEREDOC_SIGNAL)
	{
		signal(SIGINT, &signal_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig == CHILD_SIGNAL)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (sig == PARENT_SIGNAL)
	{
		signal(SIGINT, &parent_signal);
		signal(SIGQUIT, &parent_signal);
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
