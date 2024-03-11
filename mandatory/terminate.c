/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:38 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/11 21:05:17 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	del_token(void *param)
{
	t_token	*token;

	token = (t_token *)param;
	if (token)
		free(token);
}

void	free_ms(t_ms *ms)
{
	ft_lstclear(&ms->tokens, del_token);
}
