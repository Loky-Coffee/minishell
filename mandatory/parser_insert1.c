/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_insert1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:29:22 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/05 15:14:27 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*insert_cmd(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL)
		return (curr);
	if (node_is_redirect(next))
		return (insert_cmd_to_redirect(curr, next, ms));
	if (node_is_pipe(next))
	{
		if (next->left == NULL)
		{
			next->left = curr;
			curr->parent = next;
			return (next);
		}
		else if (next->left->type == NODE_REDIRECT)
			return (insert_cmd_to_redirect(curr, next, ms));
		else
			parse_error(curr->tokens[0], ms);
	}
	else if (next->type == NODE_AND || next->type == NODE_OR)
	{
		if (next->left == NULL)
		{
			next->left = curr;
			curr->parent = next;
			return (curr);
		}
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

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	i[3] = 0;
	i[4] = 0;
	while (curr->tokens[i[0]])
		i[0]++;
	while (next->left->tokens[i[1]])
		i[1]++;
	tkn_buff = ft_calloc(i[0] + i[1] + 1, sizeof(t_token *));
	if (tkn_buff == NULL)
		parse_error(curr->tokens[0], ms);
	while (curr->tokens[i[2]])
	{
		tkn_buff[i[4]] = curr->tokens[i[2]];
		i[2]++;
		i[4]++;
	}
	while (next->left->tokens[i[3]])
	{
		tkn_buff[i[4]] = next->left->tokens[i[3]];
		i[3]++;
		i[4]++;
	}
	free(next->left);
	free(curr->tokens);
	curr->tokens = tkn_buff;
}


t_node	*insert_cmd_to_redirect(t_node *curr, t_node *next, t_ms *ms)
{
	t_node	*buff;
	// t_token	**tkn_buff;
	// int		i[5];

	// i[0] = 0;
	// i[1] = 0;
	// i[2] = 0;
	// i[3] = 0;
	// i[4] = 0;
	buff = next;
	while (next->left && next->left->type == NODE_REDIRECT)
		next = next->left;
	if (next->left && (next->left->type == NODE_COMMAND \
	|| next->left->type == NODE_SUBSHELL))
	{

		// Here we check if we have to add some TOKEN_WORDs to to the command
		// cat < f1 f2 f3

		update_tokens(curr, next, ms);


		// while (curr->tokens[i[0]])
		// 	i[0]++;
		// while (next->left->tokens[i[1]])
		// 	i[1]++;
		// tkn_buff = ft_calloc(i[0] + i[1] + 1, sizeof(t_token *));
		// if (tkn_buff == NULL)
		// 	parse_error(curr->tokens[0], ms);
		// while (curr->tokens[i[2]])
		// {
		// 	tkn_buff[i[4]] = curr->tokens[i[2]];
		// 	i[2]++;
		// 	i[4]++;
		// }
		// while (next->left->tokens[i[3]])
		// {
		// 	tkn_buff[i[4]] = next->left->tokens[i[3]];
		// 	i[3]++;
		// 	i[4]++;
		// }
		// free(next->left);
		// free(curr->tokens);
		// curr->tokens = tkn_buff;
	}
	next->left = curr;
	curr->parent = next;
	return (buff);
}
