/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:25:37 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/14 11:36:33 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



int count_nodes(t_ms *ms)
{
	int i;
	t_token	*cur_token;

	i = 0;
	cur_token = ms->tokens;
	while (cur_token)
	{
		if (is_operator(cur_token->content) == 4)
			i++;
		cur_token = cur_token->next;
	}
	return ((i * 2) + 1);
}

// t_node	greate_new_node()
// {

// }

t_node	*ft_parser(t_ms *ms)
{
	t_node	**node;
	t_token	*token;
	t_node	*buffer;
	int		i;
	int		nodes_nbr;

	token =	ms->tokens;
	i = 0;
	nodes_nbr = count_nodes(ms);
	printf("note:%d\n", nodes_nbr);
	node = (t_node **)ft_calloc(nodes_nbr + 1, sizeof(t_node *));
	while (token)
	{
		while (is_operator(token->content))
		{
			buffer = (t_node *)token;
			token = token->next;
		}
		node[i] = (t_node *)ft_calloc(1, sizeof(t_node));
		node[i]->left = buffer;
		node[i]->right = (t_node *)token->next;
		// node[i]->args_left = NULL;
		// node[i]->args_right = NULL;
		node[i]->operator_type = is_operator(token->content);
		token = token->next;
		i++;
	}
	return(*node);
}
// static t_token **create_tokens(t_list *lst, int len)
// {
// 	t_list	*buf;
// 	t_token	*token;
// 	t_token **tokens;

// 	if (lst == NULL)
// 		return (NULL);
// 	buf = lst;
// 	token = (t_token *)(buf->content);
// 	if (token && (token->type == TOKEN_PIPE))
// 	{
// 		tokens = (t_token **)ft_calloc(2, sizeof(t_token *));
// 		tokens[0] = token;
// 		return (tokens);
// 	}
// 	else
// 	{
// 		while (token && (token->type != TOKEN_PIPE))
// 		{
// 			len++;
// 			buf = buf->next;
// 			if (buf)
// 				token = (t_token *)(buf->content);
// 			else
// 				token = NULL;
// 		}
// 		tokens = (t_token **)ft_calloc(len + 1, sizeof(t_token *));
// 		buf = lst;
// 		token = (t_token *)(buf->content);
// 		len = 0;
// 		while (token && (token->type != TOKEN_PIPE))
// 		{
// 			tokens[len] = token;
// 			buf = buf->next;
// 			if (buf)
// 				token = (t_token *)(buf->content);
// 			else
// 				token = NULL;
// 			len++;
// 		}
// 	}
// 	return (tokens);
// }

// t_node	*ft_parser(t_ms *ms)
// {
// 	t_list	*clst;
// 	t_token	*ct;
// 	t_node	*cnode;
// 	t_node	*last_node;
// 	t_node	*last_op;

// 	if (ms->tokens == NULL)
// 		return(NULL);
// 	last_node = NULL;
// 	cnode = NULL;
// 	clst = ms->tokens;
// 	ct = (t_token *)(clst->content);
// 	while (clst)
// 	{
// 		last_node = cnode;
// 		if (cnode && is_operator(&cnode->tokens[0]->str[cnode->tokens[0]->start]))
// 			last_op = cnode;
// 		else
// 			last_op = NULL;
// 		cnode =  (t_node *)malloc(1 * sizeof(t_node));
// 		if (cnode == NULL)
// 			// @TODO clear & free all nodes.
// 			return (NULL);
// 		// cnode->type = get_node_type(ct);
// 		if (last_op)
// 		{
// 			if (last_op->lft)
// 				last_op->rgt = cnode;
// 			cnode->lft = NULL;
// 			cnode->rgt = NULL;
// 		}
// 		else
// 		{
// 			cnode->lft = last_node;
// 			cnode->rgt = NULL;
// 		}
// 		cnode->inf = NULL;
// 		cnode->outf = NULL;
// 		cnode->pfd[0] = 0;
// 		cnode->pfd[1] = 0;
// 		cnode->tokens = create_tokens(clst, 0);
// 		if (cnode->tokens == NULL)
// 			// @TODO clear & free all nodes.
// 			return (NULL);
// 		if (cnode->tokens[0]->type == TOKEN_PIPE)
// 		{
// 			clst = clst->next;
// 			if (clst)
// 				ct = (t_token *)(clst->content);
// 			else
// 				ct = NULL;
// 		}
// 		else
// 			while (ct && (ct->type != TOKEN_PIPE))
// 			{
// 				clst = clst->next;
// 				if (clst)
// 					ct = (t_token *)(clst->content);
// 				else
// 					ct = NULL;
// 			}
// 	}
// 	return (cnode);
// }
