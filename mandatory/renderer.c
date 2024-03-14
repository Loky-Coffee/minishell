/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:50:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/14 07:58:59 by aalatzas         ###   ########.fr       */
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

// void	render_nodes(int depth, t_node *n)
// {
// 	int		i;
// 	char	*str;

// 	if (n == NULL)
// 		return;
// 	i = 0;
// 	str = NULL;
// 	while (i++ < depth)
// 		printf("  ");
// 	if (n && n->tokens&& n->tokens[0])
// 		str = ft_substr(n->tokens[0]->str, n->tokens[0]->start, n->tokens[0]->len);
// 	printf("%s\n", str);
// 	fflush(stdout);
// 	if (str)
// 		free(str);
// 	if (n->lft)
// 		render_nodes(depth + 1, n->lft);
// 	if (n->rgt)
// 		render_nodes(depth + 1, n->rgt);
// }
