NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c tokenizer.c parser.c print.c
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

