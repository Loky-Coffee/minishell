/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:41:47 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:41:48 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	ft_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->envp[i] != NULL)
		printf("%s\n", ms->envp[i++]);
	return (0);
}
