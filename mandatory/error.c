/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 18:56:31 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/20 17:40:23 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_perror(char *str)
{
	printf(LIGHTRED NINJASHELL": ");
	if (str)
		printf("%s: ", str);
	printf("%s\n"RESET, strerror(errno));
}

void	ft_cmd_error(char *msg, char *cmd, int error_code)
{
	fprintf(stderr, LIGHTRED);
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
