NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c \
	   main_helper.c \
       executor/builtin_cd.c \
       executor/builtin_echo.c \
	   executor/builtin_env.c \
	   executor/builtin_export.c \
	   executor/builtin_helper.c \
	   executor/builtin_unset.c \
	   executor/builtin.c \
	   executor/exec_heredoc.c \
	   executor/external_ex.c \
	   executor/external_helper.c \
	   executor/helper.c \
	   executor/for_quotes.c \
	   executor/pipe_ex.c \
	   executor/redirect.c \
	   executor/pipe_ex_helper.c \
	   tokenizer/tokenizer.c \
       tokenizer/tokenizer_helpers.c \
	   parser/parser.c \
	   parser/parser_helpers.c \
       expand/expanded_helpers.c \
       expand/expand_handle.c \
	   expand/get_env.c \
	   expand/expanded.c \
	   signal/signal.c \
	   free_and_checks/free.c \
	   free_and_checks/checks.c 

LIBFT = ./libft/libft.a
OBJS = $(SRCS:.c=.o)

READLINE_DIR := $(shell brew --prefix readline 2>/dev/null)
ifeq ($(READLINE_DIR),)
	RL_INC =
	RL_LIB =
else
	RL_INC = -I$(READLINE_DIR)/include
	RL_LIB = -L$(READLINE_DIR)/lib
endif
RL_LINK = -lreadline -lhistory

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(RL_INC) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL_LIB) $(RL_LINK) -o $(NAME)

$(LIBFT):
	make -C ./libft

clean:
	rm -f $(OBJS)
	make -C ./libft clean

fclean: clean
	rm -f $(NAME)
	make -C ./libft fclean

re: fclean all

