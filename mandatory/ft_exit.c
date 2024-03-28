/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 10:24:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/28 12:33:44 by nmihaile         ###   ########.fr       */
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
	long int n;
	int		sign;
	int		d;

	n = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
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
	long int n;
	int		sign;
	int		d;

	n = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
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

unsigned char	ft_exit(t_cmd *cmd, t_ms *ms)
{
	int				ac;
	unsigned char	exit_code;

	exit_code = 0;
	ac = get_ac(cmd->args);
	if (ac > 1)
	{
		if (is_long(cmd->args[1]))
		{
			if (ac > 2)
			{
				ft_error("exit", "too many arguments", NULL);
				return (1);
			}
			exit_code = ft_atol(cmd->args[1]);
		}
		else
		{
			ft_error("exit", cmd->args[1], "numeric argument required");
			exit_code = 255;
		}
	}
	ms->run = 0;
	return (exit_code);
	// The Below value have to be veryfied
}

// x exit 123 456		=> prints "exit" then prints "bash: exit: too many arguments"				=> x	return 1
// x exit 127 cd		=> prints "exit" then prints "bash: exit: too many arguments"				=> x	return 1

// x exit ab 12			=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=> ✓	exit_code: 255
// x exit abc			=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=> ✓	exit_code: 255

// x exit 1				=> prints "exit"															=> ✓	1
// x exit 0				=> prints "exit"															=> ✓	0
// x exit				=> prints "exit"															=> ✓	0
// x exit -42			=> prints "exit"															=> ✓	214
