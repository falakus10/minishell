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
# include <termios.h>
# include <unistd.h>


typedef struct s_lexer_list
{
	int							type;
	char						*token;
	int							is_next_space;
	struct s_lexer_list			*next;
}								t_lexer_list;

typedef struct s_joined_lexer_list
{
	int							type;
	char						*token;
	struct s_joined_lexer_list	*next;
}								t_joined_lexer_list;

typedef struct s_expander
{
	int							index;
	int							key_len;
	int							val_len;
	int							dollar_index;
	int							last_output;
	size_t						i;
	size_t						start;
	size_t						token_len;
	size_t						new_len;
	char						*env_key;
	char						*env_val;
	char						ch;
}								t_expander;

typedef struct s_command_block // arg count tutulmalı mı ?
{
	char *command;
	char **args;
	int *fd; //şuan tutulmuyor (kullanılmıyor)
	int cmd_count;
	int	input_fd;
	int output_fd;
	char **files;
	int *operators;            //şuan tutulmuyor (kullanılmıyor)
	char **heredoc_delimiters; // HEREDOC için kullanılacak
	int heredoc_count;         // kaç tane heredoc var
	int operator_count;
	int argument_count;
	int fd_count;
	struct s_command_block *next; // sonraki komut bloğu için
}								t_command_block;

typedef struct s_pipeline_utils
{
	int							last_token_type;
	int							is_cmd_pointed;
	int							first_token_flg;
}								t_pipeline_utils;

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
}								e_tokens;

t_lexer_list					**input_loop(void);
int								set_type(char *array);
t_lexer_list					*add_new_node(t_lexer_list **lexer_list);
void							remove_quotes(t_lexer_list *lexer_list);
t_lexer_list					**lexer_function(char *temporary_input);
int								is_quote(const char *input, int i,
									t_lexer_list *lexer_list);
int								take_word(const char *input, int i,
									t_lexer_list *lexer_list);
int								is_meta(const char *input, int i);
int								quote_len(const char *input, int start,
									char delim, t_lexer_list *lexer_list);
char							*meta_assign(const char *input, int *inx);
char							*quote_assign(const char *input, int *inx,
									t_lexer_list *lexer_list);
char							*word_assign(const char *input, int *inx,
									t_lexer_list *lexer_list);
void							ft_error(void);
void							signal_handler(void);
void							sigint_handler(int sig, siginfo_t *info,
									void *context);
void							expander(t_lexer_list *temp, char **env,
									t_expander *expander);
int								is_valid_ch(char *token, int i);
int								special_ch_check(char c);
char							*env_value(char **env, const char *key);
char							*ft_strjoin_free(char *token,
									t_expander *expander);
t_command_block					*parser(t_joined_lexer_list *list);
t_joined_lexer_list				**token_join(t_lexer_list *lexer_list);
t_joined_lexer_list				*add_new_node2(t_joined_lexer_list **lexer_list);
void							remove_quotes(t_lexer_list *lexer_list);
t_joined_lexer_list				*merge_words(t_lexer_list **temp,
									t_joined_lexer_list *current);
t_joined_lexer_list				**token_join(t_lexer_list *lexer_list);
char							**append_to_array(char **array, int count,
									char *new_value);
t_command_block					*init_command_block(void);
void							pass_cmd_blk(t_command_block **cmd,
									t_command_block **new,
									t_command_block **tmp);
void							first_pipe_ctrl(t_joined_lexer_list *temp);
void							handle_redirect_token(t_joined_lexer_list **temp,
									t_command_block **temp_block);
void							handle_token_logic(t_joined_lexer_list **tmp,
									t_command_block **tmp_blk,
									t_pipeline_utils *utils);
void							first_tkn_chck(t_pipeline_utils *utils,
									t_joined_lexer_list *temp);
int 							*append_to_array2(int *array, int count, int new_value);
int 							find_fd(char *file,t_command_block *temp);
char 							*ft_strcpy(char *dest, const char *src);
char 							*ft_strcat(char *dest, const char *src);
int								executor(t_command_block *cmd, char **env, t_expander *exp);
int								is_builtin(char *cmd);
int create_path(t_command_block *tmp_blk, char *word);

#endif