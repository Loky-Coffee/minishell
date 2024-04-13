/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:33:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/13 18:07:23 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tokentype	node_is_word(t_node *node)
{
	if (node && node->tokens && node->tokens[0]
		&& node->tokens[0]->type == TOKEN_WORD)
		return (TOKEN_WORD);
	return (NO_TOKEN);
}

t_tokentype	node_is_pipe(t_node *node)
{
	if (node && node->tokens && node->tokens[0]
		&& node->tokens[0]->type == TOKEN_PIPE)
		return (TOKEN_PIPE);
	return (NO_TOKEN);
}

t_tokentype	node_is_redirect(t_node *node)
{
	if (node && node->tokens)
		return (tkn_is_redirect(node->tokens[0]));
	return (NO_TOKEN);
}

t_tokentype	node_is_andor(t_node *node)
{
	if (node && node->tokens
		&& (node->tokens[0]->type == TOKEN_DAND \
		|| node->tokens[0]->type == TOKEN_OR))
		return (tkn_is_redirect(node->tokens[0]));
	return (NO_TOKEN);
}
