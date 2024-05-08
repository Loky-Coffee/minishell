/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:35:00 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/08 22:42:06 by nmihaile         ###   ########.fr       */
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
				break ;
			}
			i++;
		}
	}
	return (flag);
}

static int	loop_flag_args(t_cmd *cmd, int *i)
{
	int		flag;
	int		j;

	j = 1;
	*i = 1;
	flag = 0;
	while (cmd->tokens[j] && cmd->tokens[j]->str[0] == '-')
	{
		flag = set_flag(cmd->tokens[j]->str);
		if (flag)
			*i = j + 1;
		else
		{
			*i = j;
			break ;
		}
		j++;
	}
	return (flag);
}

int	ft_echo(t_cmd *cmd)
{
	int		flag;
	int		i;

	i = 0;
	flag = loop_flag_args(cmd, &i);
	while (cmd->tokens[i] != NULL)
	{
		// if (!is_operator(cmd->tokens[i]->str) \
		// && !is_single_token(*cmd->tokens[i]->str))
		printf("%s", cmd->tokens[i]->str);
		if (cmd->tokens[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
