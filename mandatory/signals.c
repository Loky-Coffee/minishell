/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:28 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/25 10:27:41 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ctrl_c_handler(int signal)
{
	if (signal == SIGINT)
	{
		// printf("\n✓ CTRL-C detected…\n");
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	init_signals(t_ms *ms)
{
	ft_memset(&ms->sa, 0, sizeof(ms->sa));
	ms->sa.sa_handler = &ctrl_c_handler;
	sigaction(SIGINT, &ms->sa, NULL);
}
