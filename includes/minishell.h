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
#include  <errno.h>


typedef struct s_lexer_list
{
	int							type;
	char						*token;
	int							is_next_space;
	struct s_lexer_list			*next;
}								t_lexer_list;

typedef struct s_mng_heredocs
{
	int index; //parser içerisinde hangi komut bloğunda olduğumu tutacak olan index. normdan dolayı struct içine açtım
	int *heredoc_flags;    
	int *heredoc_fds;
	int *heredoc_nums;
	char **heredoc_delims;

}t_mng_heredocs;

typedef struct s_env
{
	char *line;
	char *value;
	int		flag;
	struct s_env *next;
}				t_env;

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
	int							exit_value;
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
	int	status;
	int	last_output;
	int *fd;
	int *heredoc_fd;
	int cmd_count;
	pid_t pid;
	int	input_fd;
	int output_fd;
	char **files;
	int *operators;            //şuan tutulmuyor (kullanılmıyor)
	char **heredoc_delimiters; // HEREDOC için kullanılacak
	int heredoc_count;         // kaç tane heredoc var
	int operator_count;
	int argument_count;
	int fd_count;
	int err_flg;
	int err_sign;//cat <<mrb <taha<taha1 | cat <<mrb2 <taha2<taha3 | cat <<mrb3 <taha4<taha5 gibi bir girdide hata mesajında sadece ilk dosyalar yazılsın diye böyle bir flag kullandım 
	int file_err;
	int cmd_err;
	char *wrong_cmd;
	struct s_command_block *next; // sonraki komut bloğu için
}								t_command_block;

typedef struct s_pipeline_utils
{
	int							is_cmd_pointed;
}								t_pipeline_utils;

typedef struct s_executor
{
	int			*fd;
	t_expander	*exp;
	t_env		*env;
}				t_executor;


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

typedef enum e_built_in
{
	CD = 1,
	EXPORT,
	UNSET,
	EXIT,
	F_ECHO,
	PWD,
	ENV
}			e_built_in;

void							input_loop(t_command_block *command_block, t_env *env_list, char **env, t_executor *exe);
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
void expander(t_lexer_list *temp,t_env *env_list, t_expander *expander);
int								is_valid_ch(char *token, int i);
int								special_ch_check(char c);
char							*env_value(t_env *env_list, const char *key);
char							*ft_strjoin_free(char *token,
									t_expander *expander);
t_command_block					*parser(t_joined_lexer_list *list,t_mng_heredocs *mng_heredocs);

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
void							handle_redirect_token(t_joined_lexer_list **temp,
									t_command_block **temp_block,t_mng_heredocs *mng_heredocs);
void							handle_token_logic(t_joined_lexer_list **tmp,
									t_command_block **tmp_blk,
									t_pipeline_utils *utils,t_mng_heredocs *mng_heredocs);
void	loop(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils,t_mng_heredocs *mng_heredocs);
int 							*append_to_array2(int *array, int count, int new_value);
int 							find_fd(char *file,t_command_block *temp); //galiba sildim
char 							*ft_strcpy(char *dest, const char *src);
char 							*ft_strcat(char *dest, const char *src);
int								ft_strcmp(const char *s1, const char *s2);
int								executor(t_command_block *cmd, char **env, t_executor *exe);
int								is_builtin(char *cmd);
int 							create_path(t_command_block *tmp_blk, char *word);
void							make_dup(t_command_block *cmd, int index, int count, t_executor *exe);
void							create_pipe(t_command_block *cmd, t_executor *exe);
int 							multiple_exec(t_command_block *cmd, char **env, t_executor *exe);
int	change_to_env(t_lexer_list *temp, int i, t_expander *expander, t_env *env_list);
t_env **take_env(char **env);
int is_first_pipe(t_joined_lexer_list *tmp);
int just_operator(t_joined_lexer_list *tmp);
int	print_error_check(t_joined_lexer_list *tmp);
int check_tokens(t_joined_lexer_list **temp);

void	heredoc_handle(t_mng_heredocs *mng, int heredoc_count);
void	fork_or_exit(pid_t *pid);
void	create_pipe_or_exit(int fd[2]);
void	handle_parent_process(t_mng_heredocs *mng, int *fd, int *j, int *k);
void	handle_child_process(char *delim, int write_fd);

int count_cmd_blk(t_joined_lexer_list **temp);
int count_heredoc(t_joined_lexer_list **temp);
void fill_heredoc_nums(t_mng_heredocs **mng_heredocs, t_joined_lexer_list **temp);
void take_heredoc_delims(t_joined_lexer_list **temp, int heredoc_count,t_mng_heredocs **mng_heredocs);
char **free_heredoc_delimiters(char **delims, int last_index);

t_mng_heredocs *run_hrdcs(t_joined_lexer_list **temp, int cmd_blk_count);
void	fill_heredoc_flags(t_mng_heredocs *mng, t_joined_lexer_list **temp);
t_mng_heredocs *init_heredoc_struct(int count, t_joined_lexer_list **temp);
int		ft_echo(t_command_block *cmd);
int		ft_exit(t_command_block *cmd);
int		ft_cd(t_command_block *cmd, t_env *env);
int		ft_export(t_command_block *cmd, t_env  *env);
int		ft_unset(t_command_block *cmd, t_env **env);
int		ft_pwd(void);
int		ft_env(t_env *env);
int		built_in(t_command_block *cmd, t_env *env);
char	*format_export_line(t_env *node);
char	*add_quotes(char *str);
char	**env_list_to_envp(t_env *env_list);
char	*ft_strncpy(char *dest, const char *src, size_t n);

#endif
