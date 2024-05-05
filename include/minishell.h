/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:46:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/05 17:10:26 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fnmatch.h>
# include <sys/types.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include "../libft/libft.h"

/* ************************************************************************** */
// # define NINJASHELL "ninjaSHELL"
# define NINJASHELL "minishell"
# define HISTORY_FILE ".ninjaSHELL_history"
# define FALSE 0
# define TRUE 1
# define FT_PATH_MAX 4096
# define OLD_CWD 1
# define CUR_CWD 2

/* ************************************************************************** */
# ifdef DISABLE_NINJA_COLORS
#  define RED 			""
#  define GREEN			""
#  define YELLOW		""
#  define BLUE			""
#  define MAGENTA		""
#  define CYAN			""
#  define LIGHTRED		""
#  define LIGHTGREEN	""
#  define LIGHTYELLOW	""
#  define LIGHTBLUE		""
#  define LIGHTMAGENTA	""
#  define LIGHTCYAN		""
#  define RESET			""
#  define RENDER_NS_HEADER 0
# else
#  define RED			"\033[0;31m"
#  define GREEN			"\033[0;32m"
#  define YELLOW		"\033[0;33m"
#  define BLUE			"\033[0;34m"
#  define MAGENTA		"\033[0;35m"
#  define CYAN			"\033[0;36m"
#  define LIGHTRED		"\033[0;91m"
#  define LIGHTGREEN	"\033[0;92m"
#  define LIGHTYELLOW	"\033[0;93m"
#  define LIGHTBLUE		"\033[0;94m"
#  define LIGHTMAGENTA	"\033[0;95m"
#  define LIGHTCYAN		"\033[0;96m"
#  define RESET			"\033[0m"
#  define RENDER_NS_HEADER 1
# endif

/* ************************************************************************** */
// Definition of token types for a shell interpreter.
// This enumeration represents the different kinds of tokens
// that can be identified during the lexical analysis of shell commands.
// Tokens include various symbols for command parsing, redirection,
// logical operators, and environment variable handling.
typedef enum e_tokentype
{
	NO_TOKEN,
	TOKEN_WORD,
	TOKEN_SUBSHELL,
	TOKEN_PIPE,
	TOKEN_DAND,
	TOKEN_OR,
	TOKEN_LESS,
	TOKEN_GREATER,
	TOKEN_DLESS,
	TOKEN_DGREATER,
	TOKEN_TLESS,
}	t_tokentype;

/* ************************************************************************** */
// Defines the types of nodes in the Abstract Syntax Tree (AST) for shell
// command parsing.Each node type corresponds to a specific structure in the
// shell command syntax, including commands, pipes, redirections, and logical
// operators. These nodes represent the parsed structure of a command line
// input, allowing for structured interpretation and execution of shell cmd.
typedef enum e_nodetype {
	NO_NODE,
	NODE_COMMAND,
	NODE_SUBSHELL,
	NODE_REDIRECT,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
}	t_nodetype;

typedef enum e_builtin
{
	NO_BUILTIN,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT,
}	t_builtin;

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
	t_tokentype		type;
	char			*str;
	struct s_token	*next;
}					t_token;

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
	t_nodetype		type;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
	t_token			**tokens;
	int				cfd0;
	int				cfd1;
}					t_node;

/* ************************************************************************** */
typedef struct s_ms
{
	int					ac;
	char				**av;
	char				**envp;
	char				**unset_envvars;
	int					run;
	int					error;
	t_token				*tokens;
	t_node				*nodes;
	char				*line;
	char				*prompt;
	int					parse_error;
	t_token				*parse_errtkn;
	int					exit_code;
	unsigned char		shell_exit_code;
	int					hd_interupt;
	int					default_stdin;
	int					default_stdout;
}						t_ms;

/* ************************************************************************** */
typedef struct s_cmd
{
	t_token	**tokens;
	char	*cmdpth;
	char	**args;
	t_node	*node;
	char	path[FT_PATH_MAX];
}			t_cmd;

// s_expand_pattern_var
typedef struct s_epv
{
	// uint64_t		count;
	unsigned long long		count;
	DIR				*dir;
	struct dirent	*entry;
	char			pattern[FT_PATH_MAX];
}					t_epv;

/* ************************************************************************** */

// main
void			free_line(t_ms *ms);
void			free_tkn_av(t_token **tokens);
void			free_cmd(t_cmd *cmd);
void			cleanup_ms(t_ms *ms);

// Prompt
int				create_prompt(t_ms *ms);

// History
int				dump_history(t_ms *ms);
int				restore_history(t_ms *ms);

// signals.c
void			set_echoctl(int enable);
void			sigint_parent_handler(int signal, siginfo_t *siginfo, void *param);
// void			sigquit_child_handler(int signal);
void			set_signal_handler(int signal, void (handler)(int));
void			set_signal_sigaction(int signal, void (handler)(int, siginfo_t *, void *));

// Lexer
int				ft_lexer(t_ms *ms);

// expender.c
int				expand_tkn(t_token *token, t_node *node, t_ms *ms);
int				expand_node(t_node *node, t_ms *ms);

// wildcards.c
int				expand_wildcard(t_token *token);

// wildcards_utils.c
int				has_wildcards(char *pattern);
int				ft_strlchr(char *dst, const char src, size_t dstsize);
void			place_pattern(char mat[2048][2048], const char *pattern);

// wordsplitting.c
int				word_splitting(t_token *prev, t_token *token, t_token *next, int *i);

// Node_Utils1
t_nodetype		node_is_pipe(t_node *node);
t_nodetype		node_is_redirect(t_node *node);
t_nodetype		node_is_logical_operator(t_node *node);

// Node_Utils2
int				create_node(t_token **token, t_node **node);
t_node			*make_word(t_token **ct);
t_node			*make_subshell(t_token **ct);
t_node			*make_operator(t_token **ct);
t_node			*make_redirect(t_ms *ms, t_token **ct);

// Parser
t_node			*insert_node_left(t_node *curr, t_node *next);
t_node			*insert_tree_right(t_node *curr, t_node *next);
t_node			*ft_parse(t_token *ct, t_node **root, t_ms *ms);

// parser_insert1
t_node			*insert_cmd(t_node *curr, t_node *next, t_ms *ms);
t_node			*insert_cmd_to_redirect(t_node *curr, t_node *next, t_ms *ms);

// parser_insert2
t_node			*push_redirectout_into_place(t_node *curr, t_node *next);
t_node			*insert_redirect(t_node *curr, t_node *next, t_ms *ms);
t_node			*insert_pipe(t_node *curr, t_node *next, t_ms *ms);
t_node			*insert_operator(t_node *curr, t_node *next, t_ms *ms);

// parse_errors.c
void			parse_error(t_token *tkn, t_ms *ms);
int				check_for_parse_errors(t_node *node, t_ms *ms);

// Tree_Utils1
void			add_right_right(t_node **node, t_node *curr, t_node *next);
void			swap_up_righttoleft_right(t_node **node, t_node *curr, t_node *next);
void			swap_up_left(t_node **node, t_node *curr, t_node *next);
void			swap_dup_right(t_node **node, t_node *curr, t_node *next);

// Renderer
void			render_tokens(t_ms *ms);
void			render_nodes(int depth, t_node *n, char p);
void			render_ninjashell(void);
void			render_envp(char *prefix, char **envp);

// Terminate
void			del_token_content(void *param);
void			free_ms(t_ms *ms);
void			free_av(char **av);
void			ft_close_fd(int fdr, int fdw);
void			free_nodetree(t_node **n);
void			terminate(t_ms *ms, t_cmd *cmd, int exit_code);

// uToken
t_tokentype		is_single_token(char c);
t_tokentype		is_double_token(char *s);
t_tokentype		is_tripple_token(char *s);
t_tokentype		is_operator(char *s);
t_tokentype		is_word(char *str);

// uToken2
t_tokentype		is_not_word(char *s);
t_tokentype		tkn_is_operator(t_token *token);
t_tokentype		tkn_is_redirect(t_token *token);
t_tokentype		tkn_is_redirect_out(t_token *token);
t_tokentype		tkn_is_redirect_in(t_token *token);

// token_list_utils
t_token			*ft_token_new(t_tokentype type, char *str, t_token *next);
t_token			*ft_token_last(t_token *token);
void			ft_add_token_end(t_token **token, t_token *new_token);
int				tokens_size(t_token *tokens);
void			ft_token_clear(t_token **token, void (*del)(void*));

// executer
int				ft_strncmp_ignorecase(const char *s1, const char *s2, size_t n);
int				exec_intermediary(int fd_in, int fd_out, t_node *node, t_ms *ms);
int				exec_manager(t_ms *ms);

// here_doc
int				ft_heredoc(int fd_write, char *lim);
int				ft_herestring(int fd_write, char *str);

// builtins
int				cmd_is_echo_ninjashell(t_token *token);
int				run_builtin(int *fds, t_builtin builtin, t_cmd *cmd, t_ms *ms);
pid_t			fork_run_builtin(int *fds, t_builtin builtin, t_cmd *cmd, t_ms *ms);
int				ft_echo(t_cmd *cmd);
int				ft_cd(t_cmd *cmd, t_ms *ms);
int				ft_pwd(void);
int				ft_export(int i, t_node *node, t_ms *ms);
int				ft_unset(t_ms *ms);
int				ft_env(t_ms *ms);
unsigned char	ft_exit(t_cmd *cmd, t_ms *ms);

// unset_vars.c
int				ft_add_unset_envvar(char *key, t_ms *ms);
int				ft_remove_unset_envvar(char *key, t_ms *ms);

// path.c
int				ft_prepend_path(char **cmd, char *envpaths, int *exit_code);

// error.c
void			ft_error(char *s1, char *s2, char *s3);
void			ft_syntax_error(char *s1, char *s2);
void			ft_perror(char *str);
void			ft_cmd_error(char *msg, char *cmd, int error_code);


// environment.c
void			load_env(t_ms *ms, char **env);
char			*ft_env_getkey(char *str);
char			*ft_env_getvalue(char *str);
int				ft_get_env_value(t_ms *ms, char *str, char *key);
int				ft_setenv(char *key, char *value, t_ms *ms);

// environment2.c
int				is_valid_envkey(char *key);
int				add_new_index_to_envp(t_ms *ms, int len);
void			set_shlvl(t_ms *ms);

/* ************************************************************************** */

#endif
