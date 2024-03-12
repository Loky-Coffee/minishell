
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
NC		= \033[0m

NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror 			-g -fsanitize=address

SRC_DIR	= mandatory/
OBJ_DIR = mandatory/obj/

SRCS = main.c terminate.c lexer.c renderer.c utoken.c ft_whitespace.c
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))

.SILENT:

all: $(NAME)

start:
	@echo "$(YELLOW)Compiling your files, please wait a moment.$(NC)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): start libft $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	echo "$(GREEN)Compilation successful ✅$(NC)"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

t: $(NAME)
	echo "\n"
	./$(NAME)

ret: re t

.PHONY: libft



################################################################################
####                                 LIBFT                                 #####
################################################################################

LIBFT			= libft/libft.a
LIBFT_SRC_DIR	= libft/
LIBFT_OBJ_DIR	= libft/obj/

$(LIBFT_OBJ_DIR)%.o: $(LIBFT_SRC_DIR)%.c
	mkdir -p $(LIBFT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	cd libft && make

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
