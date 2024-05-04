/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:28 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/04 18:55:32 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_echoctl(int enable)
{
	struct termios	term;

	if (!isatty(STDOUT_FILENO))
		return ;
	if (tcgetattr(STDOUT_FILENO, &term) != 0)
	{
		ft_perror("tcgetattr");
		return ;
	}
	if (enable)
		term.c_lflag |= ECHOCTL;
	else
		term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &term) != 0)
	{
		ft_perror("tcsetattr");
		return ;
	}
}

void	sigint_parent_handler(int signal, siginfo_t *siginfo, void *param)
{
	static t_ms	*ms;

	if (signal == 0 && siginfo == NULL)
		ms = (t_ms *)param;
	if (signal == SIGINT)
	{
		ms->exit_code = 1;
		if (ms->line == NULL)
		{
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			write(1, "\n", 1);
	}
}

void	set_signal_handler(int signal, void (handler)(int))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(signal, &sa, NULL);
}

void	set_signal_sigaction(int signal, \
void (handler)(int, siginfo_t *, void *))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	sigaction(signal, &sa, NULL);
}
