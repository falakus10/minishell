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

char *file_path(char *file)
{
	char buf[256];
	char *full_path;

	getcwd(buf, sizeof(buf));
	full_path = malloc(ft_strlen(buf) + ft_strlen(file) + 2); // / ve \0 için +2
	if(!full_path)
	{
		printf("allocation error\n");
		ft_error(); //ne return olmalı 
	}
	ft_strcpy(full_path,buf);
	ft_strcat(full_path,"/");
	ft_strcat(full_path,file);
	return (full_path);
}

void assign_fd(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list) // açılan fd'leri close yapmadım henüz, kullanılmayacak olanları close yap
{
	char *file_pth;
	int type;

	type = (*tmp_list)->type;
	file_pth = file_path((*tmp_list)->next->token);

	if (type == HEREDOC)
	{
		(*tmp_blk)->input_fd = open("../../../../tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if ((*tmp_blk)->input_fd < 0)
		{
			perror("open");
			return;
		}
		(*tmp_blk)->heredoc_fd = append_to_array2((*tmp_blk)->heredoc_fd,(*tmp_blk)->heredoc_count,(*tmp_blk)->input_fd);
		//close((*tmp_blk)->input_fd);
	}
	else if(type == REDIR_IN)
	{
		if(access(file_pth,F_OK) == 0)
		{
			(*tmp_blk)->input_fd = open(file_pth,O_RDONLY);
			if ((*tmp_blk)->input_fd == -1)
			{
				write(2, "bash: (*tmp_list)->next->token: no such file or directory\n",59);
				//exit kodu eklenmeli $?
				ft_error(); //exit yapıyor
			}
		}
		else
		{
			write(2, "no such file or directory\n",26);
			//exit kodu eklenmeli $?
			ft_error(); //exit yapıyor
		}
	}
	else if(type == REDIR_OUT)
	{
		if(access(file_pth,F_OK) == 0)
		{
			(*tmp_blk)->output_fd = open(file_pth,O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if ((*tmp_blk)->output_fd == -1)
			{
				write(2, "no such file or directory\n",26);
				//exit kodu eklenmeli $?
				ft_error(); //exit yapıyor
			}
		}
		else
		{
			write(2, "no such file or directory\n",26);
			//exit kodu eklenmeli $?
			ft_error(); //exit yapıyor
		}
	}
	else if(type == APPEND)
	{
		if(access(file_pth,F_OK) == 0)
		{
			(*tmp_blk)->output_fd = open(file_pth, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if ((*tmp_blk)->output_fd == -1)
			{
				write(2, "bash: (*tmp)no such file or directory\n",26);
				//exit kodu eklenmeli $?
				ft_error(); //exit yapıyor
			}
		}
		else
		{
			write(2, "no such file or directory\n",26);
			//exit kodu eklenmeli $?
			ft_error(); //exit yapıyor
		}
	}
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
		assign_fd((temp_block),(temp)); //heredoc_fd
		(*temp_block)->heredoc_count++;
		*temp = (*temp)->next;
	}
	else
	{
		(*temp_block)->files = append_to_array((*temp_block)->files,
				(*temp_block)->operator_count, (*temp)->next->token);
		assign_fd((temp_block),(temp)); //fd'ler güncelleniyor ama önceki fd'ler kapanmıyor
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
	else if (((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		if (!utils->is_cmd_pointed)
		{
			if (is_builtin((*tmp)->token)) //command'e burada atama yapılacak
				(*tmp_blk)->command = ft_strdup((*tmp)->token); //tokenları direk liste olarak free'leriz o yüzden *tmp->token olarak atamayalım
			else
			{
				if(!create_path((*tmp_blk),(*tmp)->token))
				{
					printf("%s: command not found\n",(*tmp)->token);
					ft_error();
				}
			}
			(*tmp_blk)->args = append_to_array((*tmp_blk)->args,(*tmp_blk)->argument_count,(*tmp)->token);
			utils->is_cmd_pointed = 1;
			(*tmp_blk)->argument_count++;
		}
		else if (utils->is_cmd_pointed)
		{
		(*tmp_blk)->args = append_to_array((*tmp_blk)->args,
				(*tmp_blk)->argument_count, (*tmp)->token);
		(*tmp_blk)->argument_count++;
		}
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