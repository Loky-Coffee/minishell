/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/27 15:30:47 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int search_path(char **str)
{
	int i;

	i = 0;
	if (str && str[0])
	{
		while (str[0][i] != '=' && str[0][i] != '\0')
			i++;
	}
	return (i);
}

char *replace_dollar(int len, t_token *token, int pos)
{
	printf("len: %d\n", len);
	printf("token->str: %s\n", token->str);
	printf("pos: %d\n", pos);

	return(0);
}

int expander(t_ms *ms)
{
	char	str[PATH_MAX];
	int		i;
	int		j;
	char	quote_mode = '\0';

	i = 0;
	j = 0;
	ft_memset(str, 0, sizeof(str));
	while (ms->tokens->next && ms->tokens->next->str[i] != '\0')
	{
		if (ms->tokens->next->str[i] == '\"' || ms->tokens->next->str[i] == '\'')
		{
			if (quote_mode == '\0')
				quote_mode = ms->tokens->next->str[i];
			else if (quote_mode == ms->tokens->next->str[i])
				quote_mode = '\0';
			else
				str[j++] = ms->tokens->next->str[i];
		}
		else if (ms->tokens->next->str[i] == '$' && quote_mode != '\'')
			replace_dollar(search_path(&ms->tokens->next->str), ms->tokens->next, i);
		else
			str[j++] = ms->tokens->next->str[i];
		i++;
	}
	if (j != i)
	{
		free(ms->tokens->next->str);
		ms->tokens->next->str = ft_calloc((j + 1), sizeof(char));
		if (ms->tokens->next->str == NULL)
			return (1);
		ft_strlcpy(ms->tokens->next->str, str, j + 1);
	}
	if (quote_mode != '\0')
		ft_error("Syntax error: Unclosed quote detected.\n");
	return (NO_TOKEN);
}
