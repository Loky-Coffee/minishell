/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:46:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/14 11:35:44 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
/* ************************************************************************** */
# define FALSE 0
# define TRUE 1

/* ************************************************************************** */
# define RED			"\033[0;31m"
# define GREEN			"\033[0;32m"
# define YELLOW			"\033[0;33m"
# define BLUE			"\033[0;34m"
# define MAGENTA		"\033[0;35m"
# define CYAN			"\033[0;36m"
# define LIGHTRED		"\033[0;91m"
# define LIGHTGREEN		"\033[0;92m"
# define LIGHTYELLOW	"\033[0;93m"
# define LIGHTBLUE		"\033[0;94m"
# define LIGHTMAGENTA	"\033[0;95m"
# define LIGHTCYAN		"\033[0;96m"
# define RESET			"\033[0m"

/* ************************************************************************** */
// Definition of token types for a shell interpreter.
// This enumeration represents the different kinds of tokens
// that can be identified during the lexical analysis of shell commands.
// Tokens include various symbols for command parsing, redirection,
// logical operators, and environment variable handling.
typedef enum e_tokentype
{
	NO_TOKEN,		// No token identified
	TOKEN_WORD,		// A string or command
	TOKEN_SQUOTE,	// (')Single quote
	TOKEN_DQUOTE,	// (")Double quote
	TOKEN_PIPE,		// (|) PIPE
	TOKEN_LESS,		// (<) Less than symbol, used for input redirection
	TOKEN_GREATER,	// (>) Greater than symbol, used for output redirection
	TOKEN_AND,		// (&), sometimes used for background execution
	TOKEN_DOT,		// (.), used in file paths or commands like source
	TOKEN_TILDE,	// (~), represents the home directory
	TOKEN_MINUS,	// (-), used as an option prefix or in commands
	TOKEN_NEWLINE,	// (\n) New line character , separates commands
	TOKEN_DLESS,	// (<<) Double less than symbol, used for heredoc
	TOKEN_DGREATER,	// (>>) used for appending to files
	TOKEN_DAND,		// (&&), logical AND
	TOKEN_OR,		// (||), logical OR
	TOKEN_TLESS,	// (<<<), not commonly used, check if needed
	TOKEN_VARIABLE,	// e.g., $HOME), denotes environment variable usage
	TOKEN_DOLLAR,	// ($) Dollar sign , used to denote variables
} t_tokentype;
/* ************************************************************************** */

// Defines the types of nodes in the Abstract Syntax Tree (AST) for shell
// command parsing.Each node type corresponds to a specific structure in the
// shell command syntax, including commands, pipes, redirections, and logical
// operators. These nodes represent the parsed structure of a command line
// input, allowing for structured interpretation and execution of shell cmd.
typedef enum e_node_type {
	NODE_COMMAND,	// e.g., "ls -la", "wc -l", "grep hi".
	NODE_PIPE,		// "|", used to pass the out of 1 command as in to another.
	NODE_REDIRECT,	// ">", ">>" for output, and "<", "<<" for input.
	NODE_AND,		// "&&", exec the next cmd only if the curr one succeeds.
	NODE_OR			// "||", exec the next cmd only if the curr one fails.
} t_node_type;

/* ************************************************************************** */

// Defines the structure of a token in the shell
// command parsing process.
// A token represents a syntactically meaningful unit of text,
// such as a word, operator, or symbol,
// identified during the lexical analysis phase. Tokens are
// linked together to form a list, which is then used to construct
// an abstract syntax tree (AST) for parsing the shell command.
typedef struct s_token
{
	t_tokentype		type;		// The type of token look t_tokentype enum.
	char			*content;	// the command name, arg, operator, symbol.
	struct s_token	*next;		// Pointer to the next token in the list.
}   t_token;

/* ************************************************************************** */

// Defines a node in the AST, used for parsing shell commands.
//	Each node can represent a command, operator, or other syntactic element,
// holding tokens for the command or operation, arguments if applicable,
// and pointers t child nodes for constructing a hierarchical cmd structure.
// t_node structures an AST node:
// - tokens: linked list of tokenized command parts.
// - left, right: child nodes for command hierarchy.
// - args: command arguments.
// - type: nature of the node (command, operator).
typedef struct s_node
{
	t_token			*tokens;
	struct s_node	*left;
	struct s_node	*right;
	// char			**args_left;
	// char			**args_right;
	t_tokentype		operator_type;
}					t_node;
/* ************************************************************************** */

typedef struct s_ms
{
	int			run;
	int			error;
	t_token		*tokens;
	t_node		*nodes;
	char		*line;
}			t_ms;
/* ************************************************************************** */

// Lexer
void			ft_lexer(t_ms *ms);

// Parser
t_node			*ft_parser(t_ms *ms);
int				count_nodes(t_ms *ms);


// Renderer
void			render_tokens(t_ms *ms);
void			render_nodes(int depth, t_node *n);

// Terminate
void			del_token(void *param);
void			free_ms(t_ms *ms);

// uToken
t_tokentype		is_single_token(char c);
t_tokentype		is_double_token(char *s);
t_tokentype		is_tripple_token(char *s);
t_tokentype 	is_operator(char *s);
t_tokentype 	is_word(char *str);

// token_list_utils
t_token			*ft_token_new(void);
t_token			*ft_token_last(t_token *token);
void			ft_add_token_end(t_token **token, t_token *new_token);
int				tokens_size(t_token *tokens);
void			ft_token_clear(t_token **token, void (*del)(void*));
/* ************************************************************************** */

#endif