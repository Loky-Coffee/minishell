
NAME		= libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
OBJ_DIR 	= obj/
PRINTF_DIR	= printf/
GNL_DIR 	= gnl/

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c \
ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
ft_lstclear.c ft_lstiter.c ft_lstmap.c

SRCS_PRINTF	= ft_flags_bonus.c ft_helpersA_bonus.c ft_helpersB_bonus.c ft_printf_bonus.c \
ft_printf_char_bonus.c ft_printf_decimal_bonus.c ft_printf_hexadecimal_bonus.c \
ft_printf_percentage_bonus.c ft_printf_string_bonus.c ft_printf_unsigned_decimal_bonus.c \
ft_printf_void_pointer_bonus.c ft_putnbr_base_bonus.c pf_putchar_bonus.c \
pf_puthexnbr_bonus.c

SRCS_GNL	= get_next_line.c get_next_line_utils.c

OBJS 		= $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))
OBJS_PRINTF = $(addprefix $(OBJ_DIR), $(notdir $(SRCS_PRINTF:.c=.o)))
OBJS_GNL	= $(addprefix $(OBJ_DIR), $(notdir $(SRCS_GNL:.c=.o)))

.SILENT:

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_PRINTF) $(OBJS_GNL)
	ar rcs $(NAME) $(OBJS) $(OBJS_PRINTF) $(OBJS_GNL)

$(OBJ_DIR)%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(PRINTF_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(GNL_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
