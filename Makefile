NAME = minishell
SRCS = src/lexer/lexer_utils1.c src/lexer/lexer.c src/main.c src/errors.c src/signal.c src/expander/expander.c src/expander/expander_utils1.c  src/parser/parser.c src/parser/parser_utils1.c src/parser/parser_utils2.c src/utils/utils1.c src/parser/parser_utils3.c   src/executor/executor.c src/executor/multiple_exec.c

LIBFT = libft/libft.a
CC = gcc
CFLAGS =   -Iincludes -Ilibft -g

OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(READLINE) $(NAME)

$(READLINE):
	@echo "$(BOLD)$(YELLOW)[DOWNLOADING READLINE...]$(RESET)"
	@curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
	@tar -xvf readline-8.2.tar.gz
	@$(RM) readline-8.2.tar.gz
	@cd readline-8.2 && ./configure --prefix=${PWD}/lib/readline
	@cd readline-8.2 && make install
	@$(RM) readline-8.2$(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re
