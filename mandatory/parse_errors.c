/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:19:30 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/03 00:37:20 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	set_error(t_token *token, t_ms *ms)
{
	ms->parse_error = 1;
	ms->parse_errtkn = token;
}

static int	check_node(t_node *node, t_ms *ms)
{
	if (node == NULL)
		return (0);
	else if (node->type == NODE_REDIRECT)
	{
		if (node->tokens[0] && node->tokens[1] == NULL)
			return (set_error(node->tokens[0], ms), 1);
	}
	else if (node->type == NODE_PIPE)
	{
		if (node->left == NULL || node->right == NULL)
			return (set_error(node->tokens[0], ms), 1);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		if (node->left == NULL)
			return (set_error(node->tokens[0], ms), 1);
	}
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
