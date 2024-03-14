/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 00:14:48 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/14 05:07:54 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*ft_token_new(void)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->type = NO_TOKEN;
	new_token->content = NULL;
	new_token->next = NULL;
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
}

int	tokens_size(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens != NULL)
	{
		tokens = tokens->next;
		i++;
	}
	return(i);
}

void	ft_token_clear(t_token **token, void (*del)(void*))
{
	t_token	*buf;

	if (*token == NULL)
		return ;
	while (*token)
	{
		buf = (*token)->next;
		del((*token)->content);
		free(*token);
		*token = buf;
	}
}
