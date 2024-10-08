/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:39:59 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:40:01 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	ft_error(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(LIGHTRED NINJASHELL": ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s2, 2);
	}
	if (s3)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	ft_putstr_fd(RESET"\n", 2);
}

void	ft_syntax_error(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(LIGHTRED NINJASHELL": ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
	{
		ft_putstr_fd(" `", 2);
		ft_putstr_fd(s2, 2);
		ft_putstr_fd("\'", 2);
	}
	if (s3)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	ft_putstr_fd(RESET"\n", 2);
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
	ft_putstr_fd(RESET"\n", 2);
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
		ft_putstr_fd("is a directory", 2);
	else
		ft_putstr_fd("command not found", 2);
	ft_putstr_fd(RESET"\n", 2);
}

void	ft_error_braces(char *s1, char *s2)
{
	ft_putstr_fd(NINJASHELL, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(s1, 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(": bad substitution\n", 2);
}
