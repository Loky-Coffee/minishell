/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:35:00 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/18 01:44:59 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	set_flag(char *str)
{
	int	i;
	int	flag;

	flag = 0;
	if (str && str[0] == '-')
	{
		i = 1;
		while (str[i])
		{
			if (str[i] == 'n')
				flag = 1;
			else if (str[i])
			{
				flag = 0;
				break;
			}
			i++;
		}
	}
	return (flag);
}

int	ft_echo(t_cmd *cmd)
{
	int		flag;
	int		i;

	i = 1;
	if (cmd->tokens[i])
		flag = set_flag(cmd->tokens[i]->str);
	else
		flag = 0;
	if (flag == 1)
		i++;
	while (cmd->tokens[i] != NULL)
	{
		if (!is_operator(cmd->tokens[i]->str) && !is_single_token(*cmd->tokens[i]->str))
			printf("%s", cmd->tokens[i]->str);
		if(cmd->tokens[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
