/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 18:56:31 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/20 21:03:26 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_error(char *str)
{
	ft_putstr_fd(LIGHTRED NINJASHELL": ", 2);
	if (str)
		ft_putstr_fd(str, 2);
	ft_putstr_fd("\n"RESET, 2);
}

void	ft_perror(char *str)
{
	ft_putstr_fd(LIGHTRED NINJASHELL": ", 2);
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n"RESET, 2);
}

void	ft_cmd_error(char *msg, char *cmd, int error_code)
{
	ft_putstr_fd(LIGHTRED, 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	if (cmd != NULL)
		ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (error_code == 126)
		ft_putstr_fd(strerror(errno), 2);
	else
		ft_putstr_fd("command not found", 2);
	ft_putstr_fd("\n"RESET, 2);
	exit(error_code);
}
