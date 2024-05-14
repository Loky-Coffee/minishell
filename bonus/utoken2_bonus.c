/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utoken2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:29 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:30 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

t_tokentype	is_word(char *str)
{
	if (str && !ft_isspace(str[0]) && !is_not_word(str))
		return (TOKEN_WORD);
	return (NO_TOKEN);
}

t_tokentype	tkn_is_operator(t_token *token)
{
	if (token == NULL)
		return (NO_TOKEN);
	if (token->type == TOKEN_PIPE || token->type == TOKEN_DAND
		|| token->type == TOKEN_OR)
		return (token->type);
	return (NO_TOKEN);
}

t_tokentype	tkn_is_redirect(t_token *token)
{
	if (token && (token->type == TOKEN_LESS || token->type == TOKEN_GREATER \
		|| token->type == TOKEN_DLESS || token->type == TOKEN_DGREATER \
		|| token->type == TOKEN_TLESS))
		return (token->type);
	return (NO_TOKEN);
}

t_tokentype	tkn_is_redirect_in(t_token *token)
{
	if (token && (token->type == TOKEN_LESS || token->type == TOKEN_DLESS))
		return (token->type);
	return (NO_TOKEN);
}

t_tokentype	tkn_is_redirect_out(t_token *token)
{
	if (token && (token->type == TOKEN_GREATER \
		|| token->type == TOKEN_DGREATER))
		return (token->type);
	return (NO_TOKEN);
}
