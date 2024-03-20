/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:38 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/15 17:42:10 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// del_token safely frees a single token, ensuring it exists before freeing.
// It's designed for use with token list cleanup functions.
void	del_token_content(void *param)
{
	char	*str;

	str = (char *)param;
	if (str)
		free(str);
}

void	free_node(t_node **node)
{
	if ((*node)->left)
		free_node(&(*node)->left);
	if ((*node)->right)
		free_node(&(*node)->right);
	if (*node)
		free(*node);
	*node = NULL;
}

/* ************************************************************************** */
// free_ms initiates the cleanup of the entire ms structure, specifically
// clearing the token list using ft_token_clear and del_token for each token.
void	free_ms(t_ms *ms)
{
	ft_token_clear(&ms->tokens, del_token_content);
}
