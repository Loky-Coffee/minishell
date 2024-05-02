/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:24:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/03 01:03:25 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	get_ac(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

static int	is_long(char *str)
{
	long int	n;
	int			sign;
	int			d;

	if (str == NULL || *str == '\0')
		return (0);
	n = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		d = *str - '0';
		if (n == 922337203685477580 && sign == -1 && d == 8 && *(str + 1) == 0)
			return (1);
		if (n > (LONG_MAX - d) / 10)
			return (0);
		n = n * 10 + d;
		str++;
	}
	return (1);
}

static int	ft_atol(char *str)
{
	long int	n;
	int			sign;
	int			d;

	if (str == NULL || *str == '\0')
		return (0);
	n = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		d = *str - '0';
		if (n == 922337203685477580 && sign == -1 && d == 8 && *(str + 1) == 0)
			return (1);
		if (n > (LONG_MAX - d) / 10)
			return (0);
		n = n * 10 + d;
		str++;
	}
	return (n * sign);
}

unsigned char	ft_exit(t_cmd *cmd, t_ms *ms)
{
	int		ac;
	long	exit_code;

	exit_code = ms->exit_code;
	ac = get_ac(cmd->args);
	printf("exit\n");
	if (ac > 1)
	{
		if (is_long(cmd->args[1]))
		{
			if (ac > 2)
				return (ft_error("exit", "too many arguments", NULL), 1);
			exit_code = ft_atol(cmd->args[1]);
		}
		else
		{
			ft_error("exit", cmd->args[1], "numeric argument required");
			exit_code = 255;
		}
	}
	ms->run = 0;
	ms->shell_exit_code = exit_code;
	return (exit_code);
}
