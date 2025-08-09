NAME = minishell
SRCS = src/lexer/lexer_utils1.c src/lexer/lexer.c src/main.c src/errors.c src/signal.c \
src/expander/expander.c src/expander/expander_utils1.c src/parser/parser.c \
src/parser/parser_utils1.c src/parser/parser_utils2.c src/utils/utils1.c \
src/parser/parser_utils3.c src/executor/executor.c src/executor/multiple_exec.c \
src/built_in/built_in.c src/built_in/cd.c src/built_in/echo.c src/built_in/env.c \
src/built_in/exit.c src/built_in/export.c src/built_in/pwd.c src/built_in/unset.c \
src/built_in/utils.c src/utils/token_checks.c src/heredoc/run_heredocs.c \
src/heredoc/start_hrdc_struct.c src/heredoc/take_counts.c src/executor/utils.c src/utils/free.c

LIBFT = libft/libft.a
CC = cc
CFLAGS =  -Wall -Wextra -Werror -Iincludes -Ilibft -g #-fsanitize=address

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

run: all
	./$(NAME)
.PHONY: all clean fclean re
