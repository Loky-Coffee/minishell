/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:43:02 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:43:04 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	parse_error(t_token *tkn, t_ms *ms)
{
	ms->parse_error = 1;
	ms->parse_errtkn = tkn;
}

static int	check_node(t_node *node, t_ms *ms)
{
	if (node == NULL)
		return (0);
	else if (node->type == NODE_REDIRECT)
	{
		if (node->tokens[0] && node->tokens[1] == NULL)
			return (parse_error(node->tokens[0], ms), 1);
	}
	else if (node->type == NODE_PIPE)
	{
		if (node->left == NULL || node->right == NULL)
			return (parse_error(node->tokens[0], ms), 1);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		if (node->left == NULL || node->right == NULL)
			return (parse_error(node->tokens[0], ms), 1);
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
		if (error)
			return (error);
	}
	return (error);
}
