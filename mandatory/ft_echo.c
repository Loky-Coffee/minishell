/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:35:00 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/09 11:44:02 by nmihaile         ###   ########.fr       */
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

static int	set_flag(char *str)
{
	int	i;
	int	flag;
	
	flag = 0;
	if (str && str[0] == '-')
	{
		i = 1;
		while (str[i])
		{
			if (str[i] == 'n')
				flag = 1;
			else if (str[i])
			{
				flag = 0;
				break;	
			}
			i++;
		}
	}
	return (flag);
}

int	ft_echo(t_ms *ms)
{
	t_token *token;
	int		flag;

	if (ms->tokens->next)
		flag = set_flag(ms->tokens->next->str);
	else
		flag = 0;
	if (flag == 1)
		token = ms->tokens->next->next;
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
