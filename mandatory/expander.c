/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/07 00:14:14 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static int search_path(char **str)
// {
// 	int i;

// 	i = 0;
// 	if (str && str[0])
// 	{
// 		while (str[0][i] != '=' && str[0][i] != '\0')
// 			i++;
// 	}
// 	return (i);
// }

// static char	*replace_dollar(int len, t_token *token, int pos)
// {
// 	printf("len: %d\n", len);
// 	printf("token->str: %s\n", token->str);
// 	printf("pos: %d\n", pos);
// 	return (NULL);
// }

// static int	set_key(char *str, char *dst)
// {
// 	int	i;
	
// 	i = 0;
// 	ft_memset(dst, 0, FT_PATH_MAX);
// 	if (!ft_isalpha(str[i]))
// 		return (0); // $?dfg
// 	while (ft_isalnum(str[i]))
// 	{
// 		dst[i] = str[i];
// 		i++;
// 	}
// 	return (i);
// }

// len[4] has 4 lengths
// 0) The beginning of the orig str ("aaaaaaaa ")
// 1) The length of the env_var which will be put in VALUE ("HALLO")
// 2) The end lenght of the original str after the env_var (" zzzzzzz")
// 3) len of KEY => NUM=HALLO
//   "aaaaaaaa $NUM zzzzzzz"
//   "aaaaaaaa HALLO zzzzzzz"
// static int	expand_variable(int pos, t_token *token, t_ms *ms)
// {
// 	size_t	len[5];
// 	char	value[FT_PATH_MAX];
// 	char	key[FT_PATH_MAX];
// 	char	*newstr;

// 	len[0] = pos;
// 	len[3] = set_key(&token->str[pos + 1], key);
// 	ft_get_env_value(ms, value, key);
// 	len[1] = ft_strlen(value);
// 	// len[2] = ft_strlen(&token->str[pos + len[3]]);
// 	len[2] = ft_strlen(token->str) - pos - len[3];
// 	len[4] = len[0] + len[1] + len[2] + 1;
// 	newstr = (char *)ft_calloc(len[4], sizeof(char));
// 	if (newstr == NULL)
// 		return (ft_perror("Can't expande input"), 1);
// 	ft_strlcat(newstr, token->str, len[0] + 1);
// 	ft_strlcat(newstr, value, len[0] + len[1] + 1);
// 	ft_strlcat(newstr, &token->str[len[0] + len[3] + 1], len[4] + 1);
// 	free(token->str);
// 	token->str = newstr;
// 	printf("token->str |%s|\n", token->str);
// 	printf("pos:       |%d|\n", pos);
// 	printf("key:       |%s|\n", key);
// 	printf("env_var:   |%s|\n", value);
// 	return (0);
// }

static int	expand_variable(int pos, t_token *token, t_ms *ms)
{
	if (pos || token || ms) {;}
	return (0);
}

int expand_tkn(t_token *token, t_ms *ms)
{
	char	str[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode = '\0';

	i = 0;
	j = 0;
	ft_memset(str, 0, sizeof(str));
	while (token && token->str[i] != '\0')
	{
		if (token->str[i] == '\\')
		{
			i++;
			if (token->str[i])
				str[j++] = token->str[i];
		}
		else if (token->str[i] == '\"' || token->str[i] == '\'')
		{
			if (quote_mode == '\0')
				quote_mode = token->str[i];
			else if (quote_mode == token->str[i])
				quote_mode = '\0';
			else
				str[j++] = token->str[i];
		}
		else if (token->str[i] == '$' && quote_mode != '\'')
		{
			str[j++] = token->str[i];
			expand_variable(i, token, ms);
		}
		else
			str[j++] = token->str[i];
		i++;
	}
	if (j != i)
	{
		free(token->str);
		token->str = ft_calloc((j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
		ft_strlcpy(token->str, str, j + 1);
	}
	if (quote_mode != '\0')
		return (ft_error("Syntax error", "Unclosed quote detected.", NULL), 1);
	return (0);
}

int	expand_node(t_node *node, t_ms *ms)
{
	t_token	*token;

	if (node == NULL || node->tokens == NULL)
		return (1);
	token = node->tokens[0];
	while (token)
	{
		if (expand_tkn(token, ms))
			return (1);
		token = token->next;
	}
	return (0);
}
