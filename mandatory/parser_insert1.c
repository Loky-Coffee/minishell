/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_insert1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:29:22 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/06 11:24:27 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*insert_left_get_next(t_node *curr, t_node *next)
{
	next->left = curr;
	curr->parent = next;
	return (next);
}

static t_node	*insert_left_get_curr(t_node *curr, t_node *next)
{
	next->left = curr;
	curr->parent = next;
	return (curr);
}

t_node	*insert_cmd(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL)
		return (curr);
	if (node_is_redirect(next))
		return (insert_cmd_to_redirect(curr, next, ms));
	if (node_is_pipe(next))
	{
		if (next->left == NULL)
			return (insert_left_get_next(curr, next));
		else if (next->left->type == NODE_REDIRECT)
			return (insert_cmd_to_redirect(curr, next, ms));
		else
			parse_error(curr->tokens[0], ms);
	}
	else if (next->type == NODE_AND || next->type == NODE_OR)
	{
		if (next->left == NULL)
			return (insert_left_get_curr(curr, next));
		else if (next->left && next->left->type == NODE_REDIRECT)
			return (insert_cmd_to_redirect(curr, next, ms));
		else
			parse_error(curr->tokens[0], ms);
	}
	else
		parse_error(curr->tokens[0], ms);
	return (NULL);
}

void	update_tokens(t_node *curr, t_node *next, t_ms *ms)
{
	t_token	**tkn_buff;
	int		i[5];
	int		j;

	j = 0;
	while (j <= 4)
		i[j++] = 0;
	while (curr->tokens[i[0]])
		i[0]++;
	while (next->left->tokens[i[1]])
		i[1]++;
	tkn_buff = ft_calloc(i[0] + i[1] + 1, sizeof(t_token *));
	if (tkn_buff == NULL)
		parse_error(curr->tokens[0], ms);
	while (curr->tokens[i[2]])
		tkn_buff[i[4]++] = curr->tokens[i[2]++];
	while (next->left->tokens[i[3]])
		tkn_buff[i[4]++] = next->left->tokens[i[3]++];
	free(next->left);
	free(curr->tokens);
	curr->tokens = tkn_buff;
}

t_node	*insert_cmd_to_redirect(t_node *curr, t_node *next, t_ms *ms)
{
	t_node	*buff;

	buff = next;
	while (next->left && next->left->type == NODE_REDIRECT)
		next = next->left;
	if (next->left && (next->left->type == NODE_COMMAND
			|| next->left->type == NODE_SUBSHELL))
		update_tokens(curr, next, ms);
	next->left = curr;
	curr->parent = next;
	return (buff);
}
