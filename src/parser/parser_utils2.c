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

char *file_path(char *file)
{
	char buf[256];
	char *full_path;
	//file ismi $ ile başlıyorsa hata
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

void assign_fd(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list,t_mng_heredocs *mng) // açılan fd'leri close yapmadım henüz, kullanılmayacak olanları close yap
{
	char *file_pth;
	int type;
	int old_outfd;
	int old_infd;
	type = (*tmp_list)->type;
	if((*tmp_list)->next->token[0] == '$')
	{
		write(2,"bash: ",6);
		write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
		write(2,": ambiguous redirect\n",21);
		(*tmp_blk)->expnd->exit_value = 1;
		(*tmp_blk)->file_err = 1; 
	}
	file_pth = file_path((*tmp_list)->next->token);

	if(type == REDIR_IN) //dosya isminin $ ile başlama durumuna bakılabilir
	{
		if((*tmp_blk)->file_err == 0) //bu komut bloğunda henüz redirection hatası çıkmadıysa 
		{
			old_infd = (*tmp_blk)->input_fd;
			(*tmp_blk)->input_fd = open(file_pth, O_RDONLY);
			if ((*tmp_blk)->input_fd == -1)
			{
				if(errno == EISDIR)
				{		
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": Is a directory\n",17);
				}
				else if(errno == ENOENT)
				{
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": no such file or directory\n",28);
				}
				else 
					perror("bash");
				(*tmp_blk)->last_fault = 1;
				(*tmp_blk)->expnd->exit_value = 1;
				(*tmp_blk)->file_err = 1; //bu komut bloğunda file_error var demek. //Dolayısıyla bu komut bloğundaki diğer redirectionlara bakmayacak
				//flag tutulacak //eğer öncesinde input_fd hatası varsa onu da tutmalıyım ki (flag olarak olabilir) hem daha sonrasında input_fd hatası olursa onları basmasın hem de komut hatası varsa onu kontrol etmek için //bu flag sayesinde ilgili komut bloğu çalıştırılmayacak
			}
			else
			{
				if(mng->heredoc_flags[mng->index])
					close((*tmp_blk)->input_fd);				
				else if((*tmp_blk)->input_fd > -1 && old_infd != -3)
					close(old_infd);
				
			}
		}
	}
	else if(type == REDIR_OUT)
	{
		if((*tmp_blk)->file_err == 0)
		{
			old_outfd = (*tmp_blk)->output_fd; 
			(*tmp_blk)->output_fd = open(file_pth, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if ((*tmp_blk)->output_fd == -1)
			{
				if(errno == EISDIR)
				{
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": Is a directory\n",17);
				}
				else if(errno == ENOENT)
				{
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": no such file or directory\n",28);
				}
				else
					perror("bash");
				(*tmp_blk)->last_fault = 1;
				(*tmp_blk)->expnd->exit_value = 1;
				(*tmp_blk)->file_err = 1;
			}
			else
			{
				if((*tmp_blk)->output_fd > -1 && old_outfd != -3)
					close(old_outfd);
			}

		}
	}
	else if(type == APPEND)
	{
		if((*tmp_blk)->file_err == 0)
		{
			old_outfd = (*tmp_blk)->output_fd;
			(*tmp_blk)->output_fd = open(file_pth, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if ((*tmp_blk)->output_fd == -1)
			{
				if(errno == EISDIR)
				{
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": Is a directory\n",17);
				}
				else if(errno == ENOENT)
				{
					write(2,"bash: ",6);
					write(2,(*tmp_list)->next->token,ft_strlen((*tmp_list)->next->token));
					write(2,": no such file or directory\n",28);
				}
				else
					perror("bash");
				(*tmp_blk)->last_fault = 1;
				(*tmp_blk)->expnd->exit_value = 1;
				(*tmp_blk)->file_err = 1;
			}
			else
			{
				if((*tmp_blk)->output_fd > -1 && old_outfd != -3)
					close(old_outfd);
			}
		}
	}
}


void	handle_redirect_token(t_joined_lexer_list **temp,
		t_command_block **temp_block,t_mng_heredocs *mng_heredocs)
{
		if((*temp)->type == HEREDOC)
		{
			if(mng_heredocs->heredoc_flags[mng_heredocs->index]) //bu varsa redir_in ler hiç çalışmasın diyemem çünkü o zaman hata mesajını alamam (ama hata almayana kadarla sınırlasam ?)
				(*temp_block)->input_fd = mng_heredocs->heredoc_fds[mng_heredocs->index];
			*temp = (*temp)->next;
			return;
		}
		(*temp_block)->files = append_to_array((*temp_block)->files,
				(*temp_block)->operator_count, (*temp)->next->token);
		assign_fd((temp_block),(temp),mng_heredocs); //!!! fd'ler güncelleniyor ama önceki fd'ler kapanmıyor !!!
		(*temp_block)->operator_count++;
		if(mng_heredocs->heredoc_flags[mng_heredocs->index]) //bu varsa redir_in ler hiç çalışmasın diyemem çünkü o zaman hata mesajını alamam (ama hata almayana kadarla sınırlasam ?)
			(*temp_block)->input_fd = mng_heredocs->heredoc_fds[mng_heredocs->index];
		*temp = (*temp)->next;
}

//ardından son komut neden null geliyor onu çöz

void	handle_token_logic(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils,t_mng_heredocs *mng_heredocs)
{
	if ((*tmp)->token[0] == '\0' && (*tmp)->next != NULL)
		(*tmp) = (*tmp)->next;
	if ((*tmp)->next != NULL && ((*tmp)->type == REDIR_IN 		//veya != WORD && != PIPE
			|| (*tmp)->type == REDIR_OUT || (*tmp)->type == APPEND
			|| (*tmp)->type == HEREDOC))
		handle_redirect_token(tmp, tmp_blk,mng_heredocs);
	else if (((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		if (!utils->is_cmd_pointed)
		{
			if ((*tmp)->token[0] == '\0')
			{
				(*tmp) = (*tmp)->next; 
				return;
			}
			if (is_builtin((*tmp)->token)) //command'e burada atama yapılacak
			{
				(*tmp_blk)->command = ft_strdup((*tmp)->token); //tokenları direk liste olarak free'leriz o yüzden *tmp->token olarak atamayalım
			}
			/* else if(!ft_strncmp("./",(*tmp)->token,2))
			{
				(*tmp_blk)->command = ft_substr((*tmp)->token,2,ft_strlen((*tmp)->token) -2); //sonra free'lenmeli
				
			} */
			else //command ataması create_path içinde oluyor
			{
				if(!create_path((*tmp_blk),(*tmp)->token,0) && (*tmp_blk)->cmd_err == 0)
				{
					(*tmp_blk)->command = ft_strdup((*tmp)->token); 
					(*tmp_blk)->wrong_cmd = ft_strdup((*tmp)->token); //strdup ile mi atmalıyım (bence illa strdup a gerek yok parserdan sonra joined lexer list freelenebilir)!!!
					(*tmp_blk)->cmd_err = 1;
				}
			}
			(*tmp_blk)->args = append_to_array((*tmp_blk)->args,(*tmp_blk)->argument_count,(*tmp_blk)->command);
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
	(*tmp) = (*tmp)->next;
}
