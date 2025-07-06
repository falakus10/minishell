#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <readline/rltypedefs.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <termios.h>

typedef struct s_lexer_list
{
	int					type;
	char				*token;
	struct s_lexer_list	*next;
}						t_lexer_list;

typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;
}						t_env_list;

typedef enum e_tokens
{
	PIPE = 1,  // |
	REDIR_IN,  //<
	REDIR_OUT, //>
	APPEND,    //>>
	HEREDOC,   //<<
	S_QUOTE,
	D_QUOTE,
	WORD
}						e_tokens;

t_lexer_list			**input_loop(void);
int						set_type(char *array);
void					add_new_node(t_lexer_list **lexer_list, char *array);
t_lexer_list			**lexer_function(char *temporary_input);
int						is_quote(const char *input, int i);
int						take_word(const char *input, int i);
int						is_meta(const char *input, int i);
int						quote_len(const char *input, int start, char delim);
char					*meta_assign(const char *input, int *inx);
char					*quote_assign(const char *input, int *inx);
char					*word_assign(const char *input, int *inx);
void					ft_error(void);
void					signal_handler(void);
void					sigint_handler(int sig, siginfo_t *info, void *context);
void expander(t_lexer_list *list, char **env);
int is_valid_ch(t_lexer_list *list, int i);
char *env_value(char **env, const char *key);
char *ft_strjoin_free(char *env_val, char *token, const char *key);

#endif