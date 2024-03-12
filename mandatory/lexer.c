/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:45:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/12 00:05:03 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	add_new_token(t_token token_val, t_ms *ms)
{
	t_token	*token;

	if (token_val.type == TOKEN_NEWLINE)
		return (0);
	token = (t_token *)malloc(1 * sizeof(t_token));
	if (token == NULL)
		return (-1);
	token->start = token_val.start;
	token->len = token_val.len;
	token->type = token_val.type;
	token->str = ms->line;
	ft_lstadd_back(&ms->tokens, ft_lstnew((void *)token));
	return (0);
}

void	ft_lexer(t_ms *ms)
{
	static int	i;
	int			start;
	int			len;

	while (ms->line[i])
	{
		start = i;
		len = 1;
		if (is_tripple_token(&ms->line[i]))
		{
			len += 2;
			add_new_token((t_token){start, len, TOKEN_TLESS, NULL}, ms);
		}
		else if (is_double_token(&ms->line[i]))
			add_new_token((t_token){start, ++len, is_double_token(&ms->line[i]), NULL}, ms);
		else if (is_single_token(ms->line[i]))
			add_new_token((t_token){start, len, is_single_token(ms->line[i]), NULL}, ms);
		else if (ms->line[i] && ft_isalpha(ms->line[i]))
		{
			while (ft_isalpha(ms->line[i + len]))
				len++;
			add_new_token((t_token){start, len, TOKEN_WORD, NULL}, ms);
		}
		i += len;
	}
}
