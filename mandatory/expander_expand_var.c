/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expand_var.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 07:19:12 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 21:19:02 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_key(char *str, char *dst, t_ms *ms)
{
	int	i;

	i = 0;
	ft_memset(dst, 0, FT_PATH_MAX);
	if (!ft_isalpha(str[i]))
		return (0);
	while (ft_isalnum(str[i]))
	{
		dst[i] = str[i];
		i++;
		ms->i++;
	}
	ms->i--;
	return (i);
}

void	process_braces_token(t_token *token, t_ms *ms, char *str)
{
	char	*a;

	if (ft_strchr(token->str, '}'))
	{
		ms->i++;
		set_key(&token->str[ms->i], ms->key, ms);
		ft_get_env_value(ms, ms->value, ms->key);
		ft_strlcat(str, ms->value, FT_PATH_MAX);
		while (token->str[ms->i] != '}')
			ms->i++;
	}
	else
	{
		while (1)
		{
			a = readline("> ");
			if (ft_strchr(a, '}') != 0)
				break ;
		}
		if (ft_strchr(a, '}') != 0)
			ft_error_braces(&token->str[ms->i - 1], a);
		free(a);
	}
}

int	expand_parameter_and_variable(t_token *token, char *str, t_ms *ms)
{
	char	*exit_str;

	ms->i++;
	if (token->str[ms->i] == '?')
	{
		exit_str = ft_itoa(ms->exit_code);
		ft_strlcat(str, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	else if (token->str[ms->i] == '0')
	{
		exit_str = ft_strdup(NINJASHELL);
		ft_strlcat(str, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	else if (token->str[ms->i] == '{')
		process_braces_token(token, ms, str);
	else if ((token->str[0] == '~' && token->str[1] == '\0') \
	|| (token->str[0] == '~' && token->str[1] == '/'))
		return (exit_str = getenv("HOME"), \
		ft_strlcat(str, exit_str, FT_PATH_MAX), ms->i--, 0);
	set_key(&token->str[ms->i], ms->key, ms);
	ft_get_env_value(ms, ms->value, ms->key);
	ft_strlcat(str, ms->value, FT_PATH_MAX);
	return (0);
}

void	expant_parameter_utils(t_token *token, char *expstr, t_ms *ms)
{
	expand_parameter_and_variable(token, expstr, ms);
	ms->do_wildcards = 1;
	ms->j = ft_strlen(expstr);
	if (ms->quote_mode == '\0' && token->str[ms->i] == '\0')
		expstr[ms->j++] = ' ';
}

int	expant_parameter(t_token *token, t_ms *ms, char *expstr)
{
	ms->quote_mode = '\0';
	while (ms->i <= (int)ft_strlen(token->str) && token \
	&& token->str[ms->i] && token->str[ms->i] != '\0')
	{
		if (token->str[ms->i] == '\\')
			expand_single_char(expstr, &ms->j, token->str, &ms->i);
		else if (token->str[ms->i] == '\"' || token->str[ms->i] == '\'')
		{
			expand_quote(&ms->quote_mode, expstr, &ms->j, &token->str[ms->i]);
			ms->do_wildcards = 0;
		}
		else if ((token->str[ms->i] == '$' && (token->str[ms->i + 1] != '\0' \
		&& token->str[ms->i + 1] != '\"' && token->str[ms->i + 1] != ' ') \
		&& ms->quote_mode != '\'') || (token && token->str[ms->i] == '~' \
		&& ms->quote_mode == '\0' && ms->i == 0))
		{
			expant_parameter_utils(token, expstr, ms);
		}
		else
			expstr[ms->j++] = token->str[ms->i];
		ms->i++;
	}
	if (ms->quote_mode != '\0')
		return (ft_error("Syntax error", "Unclosed quote detected.", NULL), 1);
	return (0);
}
