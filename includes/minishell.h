#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
#include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
#include <readline/rltypedefs.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <dirent.h>

typedef struct s_lexer_list
{
	int type;
	char *token;

	struct s_lexer_list *next;
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

t_lexer_list	**input_loop(void);
int	set_type(char *array);
void	add_new_node(t_lexer_list **lexer_list, char *array);
t_lexer_list	**lexer_function(char *temporary_input);
int	is_quote(char *input, int i);
int	take_word(char *input, int i);
int	is_meta(char *input, int i);

#endif