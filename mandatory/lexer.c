/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 02:30:05 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 12:10:02 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	add_new_token(t_ms *ms, t_tokentype type, char *str)
{
	t_token	*new_token;

	new_token = ft_token_new(type, str, NULL);
	if (new_token == NULL)
		return (-1);
	ft_add_token_end(&(ms->tokens), new_token);
	return (0);
}

static int	check_for_operators(t_ms *ms, int *i, int start)
{
	char	*str;

	str = ft_calloc(4, sizeof(char));
	if (str == NULL)
		return (ft_perror("lexer"), -1);
	if (is_tripple_token(&ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 3 + 1);
		add_new_token(ms, TOKEN_TLESS, str);
		return (*i += 3, 1);
	}
	else if (is_double_token(&ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 2 + 1);
		add_new_token(ms, is_double_token(&ms->line[*i]), str);
		return (*i += 2, 1);
	}
	else if (is_single_token(ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 1 + 1);
		add_new_token(ms, is_single_token(ms->line[*i]), str);
		return (*i += 1, 1);
	}
	return (free(str), 0);
}

static int	handle_subshell(int i, t_ms *ms)
{
	int		len;
	int		p_mode;
	char	*str;

	len = 0;
	p_mode = 0;
	while (ms->line[i + len])
	{
		if (ms->line[i + len] == '(')
			p_mode++;
		else if (ms->line[i + len] == ')')
			p_mode--;
		len++;
		if (p_mode == 0)
			break ;
	}
	str = ft_calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (ft_perror("lexer"), -1);
	ft_strlcpy(str, &ms->line[i], len + 1);
	add_new_token(ms, TOKEN_SUBSHELL, str);
	i += len;
	return (i);
}

static int	handle_word(int i, t_ms *ms)
{
	int		len;
	int		quote_mode;
	char	*str;

	len = 0;
	while (ms->line[i + len] && !ft_isspace(ms->line[i + len]) \
	&& !is_not_word(&ms->line[i + len]))
	{
		if (ms->line[i + len] == '\"' || ms->line[i + len] == '\'')
		{
			quote_mode = ms->line[i + len];
			len++;
			while (ms->line[i + len] && ms->line[i + len] != quote_mode)
				len++;
			if (ms->line[i + len])
				len++;
		}
		else
			len++;
	}
	str = ft_calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (ft_perror("lexer"), -1);
	ft_strlcpy(str, &ms->line[i], len + 1);
	return (add_new_token(ms, TOKEN_WORD, str), i += len, i);
}

int	ft_lexer(t_ms *ms)
{
	int		oi;
	int		i;

	if (ms->line && ms->line[0] == '#')
		return (1);
	i = 0;
	while (ms->line && ms->line[i])
	{
		oi = check_for_operators(ms, &i, i);
		if (oi == -1)
			return (1);
		if (oi >= 1)
			continue ;
		else if (ms->line[i] && ms->line[i] == '(')
			i = handle_subshell(i, ms);
		else if (ms->line[i] && !ft_isspace(ms->line[i]))
			i = handle_word(i, ms);
		else
			i++;
		if (i == -1)
			return (1);
	}
	return (0);
}
