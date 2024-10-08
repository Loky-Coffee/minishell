/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:34:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/04 17:58:53 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_node(t_token **token, t_node **node)
{
	t_node	*n;

	if (*token == NULL)
		return (1);
	n = (t_node *)ft_calloc(1, sizeof(t_node));
	if (n == NULL)
		return (1);
	n->type = NO_NODE;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->tokens = NULL;
	n->cfd0 = -1;
	n->cfd1 = -1;
	*node = n;
	return (0);
}

t_node	*make_word(t_token **ct)
{
	t_node	*n;
	t_token	*t;
	int		i;

	if (create_node(ct, &n))
		return (NULL);
	n->type = NODE_COMMAND;
	t = *ct;
	i = 0;
	while (t && !tkn_is_operator(t) && !tkn_is_redirect(t))
	{
		i++;
		t = t->next;
	}
	n->tokens = (t_token **)ft_calloc(i + 1, sizeof(t_token));
	if (n->tokens == NULL)
		return (free(n), NULL);
	i = 0;
	while (*ct && !tkn_is_operator(*ct) && !tkn_is_redirect(*ct))
	{
		n->tokens[i++] = *ct;
		*ct = (*ct)->next;
	}
	return (n);
}

t_node	*make_subshell(t_token **ct)
{
	t_node	*n;

	if (create_node(ct, &n))
		return (NULL);
	n->type = NODE_SUBSHELL;
	n->tokens = (t_token **)ft_calloc(2, sizeof(t_token));
	if (n->tokens == NULL)
	{
		free(n);
		return (NULL);
	}
	n->tokens[0] = (*ct);
	*ct = (*ct)->next;
	return (n);
}

t_node	*make_operator(t_token **ct)
{
	t_node	*n;

	if (create_node(ct, &n))
		return (NULL);
	n->tokens = (t_token **)ft_calloc(2, sizeof(t_token));
	if (n->tokens == NULL)
	{
		free(n);
		return (NULL);
	}
	if ((*ct)->type == TOKEN_PIPE)
		n->type = NODE_PIPE;
	else if ((*ct)->type == TOKEN_DAND)
		n->type = NODE_AND;
	else if ((*ct)->type == TOKEN_OR)
		n->type = NODE_OR;
	else
		n->type = NO_NODE;
	n->tokens[0] = *ct;
	*ct = (*ct)->next;
	return (n);
}

t_node	*make_redirect(t_ms *ms, t_token **ct)
{
	int		i;
	t_node	*n;

	if (create_node(ct, &n))
		return (NULL);
	i = 1;
	if ((*ct)->next && (*ct)->next->type == TOKEN_WORD)
		i++;
	else
		return (parse_error((*ct), ms), NULL);
	n->tokens = (t_token **)ft_calloc(i + 1, sizeof(t_token));
	if (n->tokens == NULL)
		return (free(n), NULL);
	n->type = NODE_REDIRECT;
	n->tokens[0] = *ct;
	if (i == 2)
	{
		*ct = (*ct)->next;
		n->tokens[1] = *ct;
	}
	*ct = (*ct)->next;
	return (n);
}
