NAME = minishell
SRCS = src/lexer/lexer_utils2.c src/lexer/lexer_utils1.c src/lexer/lexer.c src/main_init/main.c src/main_init/signal.c \
src/expander/expander.c src/expander/expander_utils1.c src/parser/parser.c \
src/parser/parser_utils1.c src/parser/parser_utils2.c src/utils/utils1.c \
src/parser/parser_utils3.c src/parser/parser_utils4.c src/executor/executor.c src/executor/multiple_exec.c \
src/built_in/built_in.c src/built_in/cd.c src/built_in/echo.c src/built_in/env.c src/parser/token_logic.c\
src/built_in/exit.c src/built_in/export.c src/built_in/pwd.c src/built_in/unset.c \
src/built_in/utils.c src/utils/token_checks.c src/heredoc/run_heredocs.c \
src/heredoc/start_hrdc_struct.c src/heredoc/take_counts.c src/executor/utils.c src/utils/free.c\
src/parser/assign_fd.c src/expander/expander_utils2.c src/utils/utils2.c src/utils/utils3.c \
src/utils/free2.c src/heredoc/heredoc_handle.c src/main_init/init.c src/main_init/init2.c src/main_init/init3.c

LIBFT = libft/libft.a
CC = cc
CFLAGS =  -Wall -Wextra -Werror -Iincludes -Ilibft

OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

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