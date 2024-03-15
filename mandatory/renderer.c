/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:50:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/15 17:14:14 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// The render_tokens function iterates through the token list in ms,
// printing each token's type and content to the console. It provides a
// visual representation of the parsed tokens for debugging or inspection.
void	render_tokens(t_ms *ms)
{
	t_token	*token;

	token = ms->tokens;
	while (token != NULL)
	{
		printf("type: [%u] content: [%s]\n", token->type, token->content);
		printf("-------------------------\n");
		token = token->next;
	}
}

static void	render_doubly_token(t_token **tokens)
{
	int		i;

	if (tokens == NULL)
		return ;
	i = 0;
	while (tokens[i])
	{
		printf("%s ", tokens[i]->content);
		i++;
	}
	printf("\n");
}

void	render_nodes(int depth, t_node *n, char p)
{
	int		i;

	if (n == NULL)
		return;
	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("(%c) ", p);
	if (n && n->tokens && n->tokens[0])
		render_doubly_token(n->tokens);
	if (n->left)
		render_nodes(depth + 1, n->left, '<');
	if (n->right)
		render_nodes(depth + 1, n->right, '>');
}
