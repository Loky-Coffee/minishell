/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 23:27:02 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/27 23:27:59 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->envp[i] != NULL)
		printf("%s\n", ms->envp[i++]);
	return (0);
}
