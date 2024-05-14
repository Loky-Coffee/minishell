/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:43:58 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:43:59 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

t_token	*ft_token_new(t_tokentype type, char *str, t_token *next)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->type = type;
	new_token->str = str;
	new_token->next = next;
	return (new_token);
}

t_token	*ft_token_last(t_token *token)
{
	if (token == NULL)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

void	ft_add_token_end(t_token **token, t_token *new_token)
{
	t_token	*last_token;

	if (*token == NULL)
		*token = new_token;
	else
	{
		last_token = ft_token_last(*token);
		last_token->next = new_token;
	}
	new_token->next = NULL;
}

int	tokens_size(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens != NULL)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	ft_token_clear(t_token **token, void (*del)(void*))
{
	t_token	*buf;

	if (*token == NULL)
		return ;
	while (*token)
	{
		buf = (*token)->next;
		del((*token)->str);
		free(*token);
		*token = buf;
	}
}
