#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <dirent.h>

typedef struct s_lexer_list
{
	int type;
	char *token;
	t_lexer_list *next;
}t_lexer_list;

typedef enum e_tokens
{
	PIPE = 1, // |
	REDIR_IN, //<
	REDIR_OUT, //>
	APPEND,  //>>
	HEREDOC, //<<
	S_QUOTE, 
	D_QUOTE,
	WORD
}e_tokens;


#endif