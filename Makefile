# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/20 16:47:30 by aalatzas          #+#    #+#              #
#    Updated: 2024/05/14 22:23:26 by aalatzas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
NC		= \033[0m

NAME	= minishell
NAME_BONUS = minishell_bonus
CC		= cc
CFLAGS	= -Wall -Wextra -Werror  			-g # -fsanitize=address
NCOLORS =  -DDISABLE_NINJA_COLORS
SRC_DIR	= mandatory/
SRC_DIR_BONUS	= bonus/
OBJ_DIR = mandatory/obj/
OBJ_DIR_BONUS = bonus/obj/
LIBFT			= libft/libft.a
LIBFT_SRC_DIR	= libft/
LIBFT_OBJ_DIR	= libft/obj/

SRCS = main.c utils.c user_input.c exec_arguments.c prompt.c history.c terminate.c lexer.c parser.c parser_insert1.c \
parser_insert2.c parser_insert3.c parse_errors.c renderer.c \
utoken.c utoken2.c token_list_utils.c expander.c expander_expand_var.c expander_utils.c expander_utils2.c wildcards.c wildcards_utils.c wordsplitting.c wordsplitting_utils.c \
executer.c executer_utils1.c executer_create_cmds.c executer_check_cmd.c executer_builtins.c \
executer_exec_cmd.c executer_redirects.c executer_operators.c \
path.c error.c signals.c environment.c environment_utils.c unset_vars.c \
here_doc.c here_doc2.c node_utils1.c node_utils2.c tree_utils1.c \
ft_echo.c ft_cd.c ft_pwd.c ft_export.c ft_export_utils1.c ft_export_utils2.c ft_unset.c ft_env.c ft_exit.c

SRCS_BONUS = main_bonus.c utils_bonus.c user_input_bonus.c exec_arguments_bonus.c prompt_bonus.c history_bonus.c terminate_bonus.c lexer_bonus.c parser_bonus.c parser_insert1_bonus.c \
parser_insert2_bonus.c parser_insert3_bonus.c parse_errors_bonus.c renderer_bonus.c \
utoken_bonus.c utoken2_bonus.c token_list_utils_bonus.c expander_bonus.c expander_expand_var_bonus.c expander_utils_bonus.c expander_utils2_bonus.c wildcards_bonus.c wildcards_utils_bonus.c wordsplitting_bonus.c wordsplitting_utils_bonus.c \
executer_bonus.c executer_utils1_bonus.c executer_create_cmds_bonus.c executer_check_cmd_bonus.c executer_builtins_bonus.c \
executer_exec_cmd_bonus.c executer_redirects_bonus.c executer_operators_bonus.c \
path_bonus.c error_bonus.c signals_bonus.c environment_bonus.c environment_utils_bonus.c unset_vars_bonus.c \
here_doc_bonus.c here_doc2_bonus.c node_utils1_bonus.c node_utils2_bonus.c tree_utils1_bonus.c \
ft_echo_bonus.c ft_cd_bonus.c ft_pwd_bonus.c ft_export_bonus.c ft_export_utils1_bonus.c ft_export_utils2_bonus.c ft_unset_bonus.c ft_env_bonus.c ft_exit_bonus.c

OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))
OBJS_BONUS = $(addprefix $(OBJ_DIR_BONUS), $(notdir $(SRCS_BONUS:.c=.o)))

all: $(NAME)

bonus: $(NAME_BONUS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(NCOLORS) -c $< -o $@

$(OBJ_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c
	@mkdir -p $(OBJ_DIR_BONUS)
	@$(CC) $(CFLAGS) $(NCOLORS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(NCOLORS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	@echo "$(GREEN)Compilation successful ✅$(NC)"

$(NAME_BONUS): $(LIBFT) $(OBJS_BONUS)
	@$(CC) $(CFLAGS) $(NCOLORS) $(OBJS_BONUS) $(LIBFT) -o $(NAME_BONUS) -lreadline
	@echo "$(GREEN)Compilation successful ✅$(NC)"


clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_DIR_BONUS)

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: fclean all

t: $(NAME)
	echo "\n"
	./$(NAME)

ret: re t

.PHONY: libft



################################################################################
####                                 LIBFT                                 #####
################################################################################

$(LIBFT_OBJ_DIR)%.o: $(LIBFT_SRC_DIR)%.c
	@mkdir -p $(LIBFT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@cd libft && make

clean_libft:
	cd libft && make clean

fclean_libft: clean_libft
	cd libft && make fclean

re_libft:
	cd libft && make re



################################################################################
####                             GLOBAL  RULES                             #####
################################################################################

fcleanall: fclean clean_libft  fclean_libft
	rm -f libft.a
