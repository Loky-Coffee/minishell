/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:35:00 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/07 18:32:44 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int search_operator(t_ms *ms)
{
	t_token *tokens;
	int i;

	tokens = ms->tokens;
	while (tokens != NULL)
	{
		i = 0;
		while (tokens->str[i] != 0)
		{
			if (is_operator(&tokens->str[i]))
				return (TOKEN_PIPE);
			i++;
		}
		tokens = tokens->next;
	}
	return (NO_TOKEN);
}

int search_dollar(t_ms *ms)
{
	t_token *tokens;
	int i;

	tokens = ms->tokens;
	while (tokens != NULL)
	{
		i = 0;
		while (tokens->str[i] != 0)
		{
			if (tokens->str[i] == '$')
				return (TOKEN_DOLLAR);
			i++;
		}
		tokens = tokens->next;
	}
	return (NO_TOKEN);
}

int	ft_echo(t_ms *ms)
{
	t_token *token;
	int		flag;

	flag = 0;
	if (ms->tokens->next && ft_strncmp("-n", ms->tokens->next->str, 3) == 0)
	{
		token = ms->tokens->next->next;
		flag = 1;
	}
	else
		token = ms->tokens->next;
	while (token != NULL)
	{
		if (!is_operator(token->str) && !is_single_token(*token->str))
			printf("%s", token->str);
		if(token->next)
			printf(" ");
		token = token->next;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
