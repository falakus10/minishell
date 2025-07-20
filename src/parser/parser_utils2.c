#include "minishell.h"

void	pass_cmd_blk(t_command_block **cmd, t_command_block **new,
		t_command_block **tmp)  //sonraki komut bloğuna geçiyor
{
	if ((*cmd) == NULL)
	{
		(*cmd) = (*new);
		(*tmp) = (*cmd);
	}
	else
	{
		(*tmp)->next = (*new);
		(*tmp) = (*tmp)->next;
	}
}

void	first_pipe_ctrl(t_joined_lexer_list *temp)
{
	if (temp->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		ft_error();
	}
}

int find_fd(char *file,t_command_block *temp_block)
{
	int fd;
	char buf[256];
	char *full_path;
	int fd_cnt;

	fd_cnt = temp_block->fd_count;
	getcwd(buf, sizeof(buf));
	full_path = malloc(ft_strlen(buf) + ft_strlen(file) + 2);
	if(!full_path)
	{
		printf("allocation error\n");
		return (1); //ne return olmalı 
	}
	ft_strcpy(full_path,buf);
	ft_strcat(full_path,"/");
	ft_strcat(full_path,file);
	fd = open(full_path, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
    	return -1;
	}
	close(fd);
	temp_block->fd_count++;
	return (fd + (fd_cnt));
}

void	handle_redirect_token(t_joined_lexer_list **temp,
		t_command_block **temp_block)
{
	if ((*temp)->next->type != WORD && (*temp)->next->type != S_QUOTE
		&& (*temp)->next->type != D_QUOTE)
	{
		printf("Syntax error: Operator must be followed by a word\n");
		ft_error();
	}
	if ((*temp)->type == HEREDOC)
	{
		(*temp_block)->heredoc_delimiters = append_to_array((*temp_block)->heredoc_delimiters,
				(*temp_block)->heredoc_count, (*temp)->next->token);
		(*temp_block)->heredoc_count++;
		*temp = (*temp)->next;
	}
	else
	{
		(*temp_block)->files = append_to_array((*temp_block)->files,
				(*temp_block)->operator_count, (*temp)->next->token);
		(*temp_block)->fd = append_to_array2((*temp_block)->fd,
				(*temp_block)->operator_count, find_fd((*temp_block)->files[(*temp_block)->operator_count],(*temp_block)));
		(*temp_block)->operator_count++;
		*temp = (*temp)->next;
	}
}

void	handle_token_logic(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils)
{
	int fd_count;

	fd_count = 0;
	if ((*tmp)->next != NULL && ((*tmp)->type == REDIR_IN 		//veya != WORD && != PIPE
			|| (*tmp)->type == REDIR_OUT || (*tmp)->type == APPEND
			|| (*tmp)->type == HEREDOC))
		handle_redirect_token(tmp, tmp_blk);
	else if (!(utils->is_cmd_pointed) && ((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		(*tmp_blk)->command = ft_strdup((*tmp)->token);
		utils->is_cmd_pointed = 1;
	}
	else if (utils->is_cmd_pointed && ((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		(*tmp_blk)->args = append_to_array((*tmp_blk)->args,
				(*tmp_blk)->argument_count, (*tmp)->token);
		(*tmp_blk)->argument_count++;
	}
	fd_count++;
	utils->last_token_type = (*tmp)->type;
	(*tmp) = (*tmp)->next;
}

void	first_tkn_chck(t_pipeline_utils *utils, t_joined_lexer_list *temp)
{
	if (utils->first_token_flg == 0 && temp->next == NULL
		&& (temp->type == REDIR_IN || temp->type == REDIR_OUT
			|| temp->type == APPEND || temp->type == HEREDOC))
	{
		printf("Syntax error: Operator cannot be at the end\n");
		ft_error();
	}
}