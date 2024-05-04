/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:50:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/04 18:57:20 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	render_tokens(t_ms *ms)
{
	t_token	*token;

	token = ms->tokens;
	while (token != NULL)
	{
		printf("type: [%u] str: [%s]\n", token->type, token->str);
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
		printf("[%s] ", tokens[i]->str);
		i++;
	}
	printf("\n");
}

void	render_nodes(int depth, t_node *n, char p)
{
	int		i;

	if (n == NULL)
		return ;
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

void	render_ninjashell(void)
{
	if (RENDER_NS_HEADER)
	{
		printf(LIGHTRED
			"        •  •  ┏┓┓┏┏┓┓ ┓                     \n"
			"      ┏┓┓┏┓┓┏┓┗┓┣┫┣ ┃ ┃       Hack it away …\n"
			"━━━━━━┛┗┗┛┗┃┗┻┗┛┛┗┗┛┗┛┗┛━━━━━━━━━━━━━━━━━━━━\n"
			"           ┛                                \n"
			RESET);
		printf("\n");
	}
}

void	render_envp(char *prefix, char **envp)
{
	size_t	i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		printf("%s", prefix);
		key = ft_env_getkey(envp[i]);
		value = ft_env_getvalue(envp[i]);
		if (key)
		{
			printf("%s", key);
			if (value)
				printf("=\"%s\"", value);
			free(key);
		}
		if (value)
			free(value);
		printf("\n");
		i++;
	}
}
