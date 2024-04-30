/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:19:30 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/30 11:42:44 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_node(t_node *node, t_ms *ms)
{
	if (node == NULL)
		return (0);
	// if (node->type == NODE_COMMAND)
	// 	;
	// else if (node->type == NODE_SUBSHELL)
	// 	;
	// else if (node->type == NODE_REDIRECT)
	// 	;
	else if (node->type == NODE_PIPE)
	{
		if (node->left == NULL || node->right == NULL)
		{
			ms->parse_error = 1;
			ms->parse_errtkn = node->tokens[0];
			return (1);
		}
	}
	// else if (node->type == NODE_AND || node->type == NODE_OR)
	// 	;
	return (0);
}

int	check_for_parse_errors(t_node *node, t_ms *ms)
{
	int	error;

	error = 0;
	if (node == NULL)
		return (error);
	error = check_node(node, ms);
	if (error)
		return (error);
	else
	{
		error = check_for_parse_errors(node->left, ms);
		if (error)
			return (error);
		error = check_for_parse_errors(node->right, ms);
	}
	return (error);
}
