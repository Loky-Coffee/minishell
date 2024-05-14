/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utoken_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:22 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:24 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

t_tokentype	is_single_token(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_LESS);
	else if (c == '>')
		return (TOKEN_GREATER);
	return (NO_TOKEN);
}

t_tokentype	is_double_token(char *s)
{
	if (ft_strncmp(s, "<<", 2) == 0)
		return (TOKEN_DLESS);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (TOKEN_DGREATER);
	if (ft_strncmp(s, "&&", 2) == 0)
		return (TOKEN_DAND);
	if (ft_strncmp(s, "||", 2) == 0)
		return (TOKEN_OR);
	return (NO_TOKEN);
}

t_tokentype	is_tripple_token(char *s)
{
	if (ft_strlen(s) >= 3)
		if (s[0] == '<' && s[1] == '<' && s[2] == '<')
			return (TOKEN_TLESS);
	return (NO_TOKEN);
}

t_tokentype	is_operator(char *s)
{
	if (is_tripple_token(s))
		return (is_tripple_token(s));
	else if (is_double_token(s))
		return (is_double_token(s));
	else if (is_single_token(*s) == TOKEN_PIPE \
	|| is_single_token(*s) == TOKEN_LESS \
	|| is_single_token(*s) == TOKEN_GREATER)
		return (is_single_token(*s));
	return (NO_TOKEN);
}

t_tokentype	is_not_word(char *s)
{
	if (is_tripple_token(s))
		return (is_tripple_token(s));
	else if (is_double_token(s))
		return (is_double_token(s));
	else if (is_single_token(*s))
		return (is_single_token(*s));
	return (NO_TOKEN);
}
