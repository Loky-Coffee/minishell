/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:38 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/14 07:59:16 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// del_token safely frees a single token, ensuring it exists before freeing.
// It's designed for use with token list cleanup functions.
void	del_token(void *param)
{
	t_token	*token;

	token = (t_token *)param;
	if (token)
		free(token);
}

/* ************************************************************************** */
// free_ms initiates the cleanup of the entire ms structure, specifically
// clearing the token list using ft_token_clear and del_token for each token.
void	free_ms(t_ms *ms)
{
	ft_token_clear(&ms->tokens, del_token);
}
