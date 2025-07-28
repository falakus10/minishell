#include "minishell.h"

t_lexer_list	**input_loop(void)
{
	char *input;
	char *temp_input;
	t_lexer_list **list;

	while (1)
	{
		temp_input = readline("minishell>"); //temp_input yerine input kullanamayız çünkü readline'dan dönen alanı kaybederiz, leak çıkar.
		if (temp_input == NULL)
		{
			free(temp_input); //NULL şeyi freelemek saçma
			write(1, "exit\n", 5);
			exit(0); //0 mı 1 mi
		}
		if (temp_input[0] == '\0')
    	{
        free(temp_input);
        continue;
    	}
		input = ft_strtrim(temp_input, " ");
		list = lexer_function(input);
		free(temp_input); // bununla işimiz bitti
		// input'u da işimiz bitince free'lemeliyiz
		break;
	}
	return (list);
}

void file_cntrl(t_command_block *iter,t_command_block *last_error_block)
{
	while(iter != NULL) //burası file_control fonksiyonu. Eğer heredoc_count 0 ise, sadece bu fonksiyon çalışısn diyebiliriz.
	{
		if (iter->err_flg != -2) //>$ hatası için err_flg -3 tutulup hata mesajları ona göre özelleştirilebilir.
		{
			printf("bash: no such file or directory %s\n", iter->files[iter->err_flg]);
			if (iter == last_error_block)
				ft_error(); // sadece sonuncusunda çık
		}
	iter = iter->next;
	}
}

void	heredoc_handle(t_mng_heredocs *mng, int heredoc_count)
{
	int		i;
	int		fd[2];
	pid_t	pid;
	char	*line;
	int j = 0;
	int k = 0;
	for (i = 0; i < heredoc_count; i++)
	{
	
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) // child
		{
			close(fd[0]); // read ucunu kapat
			while (1)
			{
				line = readline("> ");
				if (!line || ft_strcmp(line, mng->heredoc_delims[i]) == 0)
				{
					free(line);
					break;
				}
				write(fd[1], line, ft_strlen(line));
				write(fd[1], "\n", 1);
				free(line);
			}
			close(fd[1]);
			exit(0);
		}
		else // parent
		{
			close(fd[1]); // write ucunu kapat
			waitpid(pid, NULL, 0);

			if(k < mng->heredoc_nums[j])
				k++;
			else
			{
				if(mng->heredoc_flags[j])
				mng->heredoc_fds[j] = fd[0];
				k = 0;
				j++; 
			}
		}
	}
}

int count_heredoc(t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	int heredoc_count;

	tmp = *temp;
	heredoc_count = 0;
	while(tmp != NULL)
	{
		if(tmp->type == HEREDOC)
			heredoc_count++;
		tmp = tmp->next;
	}
	return (heredoc_count);
}

int count_cmd_blk(t_joined_lexer_list **temp)
{
	int pipe_count;
	t_joined_lexer_list *tmp;

	pipe_count = 0;
	tmp = (*temp);
	while((tmp) != NULL) //tmp->next != NULL da olur ama böyle kalabilir 
	{
		if(tmp->type == PIPE)
			pipe_count++;
		tmp=tmp->next;
	}
	return (pipe_count + 1);
}


char **free_heredoc_delimiters(char **delims, int last_index) //gerek var mı ?
{
	while (--last_index >= 0)
		free(delims[last_index]);
	free(delims);
	return (NULL);
}


void take_heredoc_delims(t_joined_lexer_list **temp, int heredoc_count,t_mng_heredocs **mng_heredocs)
{
	t_joined_lexer_list *tmp;
	int i = 0;
	tmp = *temp;
	while (tmp && i < heredoc_count)
	{
		if (tmp->type == HEREDOC && tmp->next && tmp->next->token)
		{
			(*mng_heredocs)->heredoc_delims[i] = ft_strdup(tmp->next->token);
			if (!(*mng_heredocs)->heredoc_delims[i])
				free_heredoc_delimiters((*mng_heredocs)->heredoc_delims, i); //gerek var mı 
			i++;
		}
		tmp = tmp->next;
	}
	(*mng_heredocs)->heredoc_delims[i] = NULL;
}

void fill_heredoc_nums(t_mng_heredocs **mng_heredocs,t_joined_lexer_list **temp,int heredoc_count)
{
	t_joined_lexer_list *tmp;
	int i;

	tmp = *temp;
	i = 0;
	while(tmp != NULL)
	{
		if((tmp->type == HEREDOC))
		{
			(*mng_heredocs)->heredoc_nums[i]++;
			if(tmp->next->next == NULL)
				break;
			else if (tmp->next->next->type == PIPE)
				i++;
		}
		tmp=tmp->next;
	}
}

t_mng_heredocs *fill_hrdc_flgs(t_joined_lexer_list **temp, int cmd_blk_count) 
{
	t_mng_heredocs *mng_heredocs;
	t_joined_lexer_list *tmp;
	int i;

	mng_heredocs = malloc(sizeof(mng_heredocs));
	mng_heredocs->heredoc_flags = malloc(sizeof(int) * cmd_blk_count);
	mng_heredocs->heredoc_fds = malloc(sizeof(int) *  cmd_blk_count);
	mng_heredocs->heredoc_nums = malloc(sizeof(int) *cmd_blk_count);
	mng_heredocs->heredoc_delims = malloc(sizeof(char *) * (count_heredoc(temp) + 1));
	tmp = *temp;

	ft_memset(mng_heredocs->heredoc_flags,0,sizeof(mng_heredocs->heredoc_flags));
	ft_memset(mng_heredocs->heredoc_fds,-1,sizeof(mng_heredocs->heredoc_fds));
	ft_memset(mng_heredocs->heredoc_nums,0,sizeof(mng_heredocs)->heredoc_fds);
	i = 0;
	while(tmp != NULL)
	{
		if((tmp->type == HEREDOC) && (tmp->next != NULL))
		{
			if(tmp->next->next == NULL) //son komut bloğunun sonu << delim şeklinde bitiyorsa
				mng_heredocs->heredoc_flags[i++] = 1;
			else if(tmp->next->next->type == PIPE) //komut bloğunun sonu << delim şeklinde bitiyorsa
				mng_heredocs->heredoc_flags[i++] = 1;
		}
		tmp = tmp->next;
	}
	take_heredoc_delims(temp,count_heredoc(temp),&mng_heredocs);
	fill_heredoc_nums(&mng_heredocs,temp,count_heredoc(temp));
	heredoc_handle(mng_heredocs,count_heredoc(temp));
	return (mng_heredocs);
}

int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	t_lexer_list	*temp;
	t_lexer_list	**list;
	t_env			**env_list;
	t_expander		*exp;
	//t_env *tmp;
	//t_command_block	*command_block;
	t_joined_lexer_list **new_list;
	t_mng_heredocs *mng_hereodocs;
	exp = malloc(sizeof(t_expander));
	if (!exp)
		return (0);
	signal_handler();
	list = input_loop(); //bu listede lexer'da ayrılmış olan token'ları tutuyoruz 
	temp = *list;	
	env_list = take_env(env);
	//tmp = *env_list;  //tmp e gerek var mı ?

 	expander(temp, *env_list, exp);
	remove_quotes(*list);
	new_list = token_join(temp);
	check_tokens(new_list);
	mng_hereodocs=fill_hrdc_flgs(new_list,count_cmd_blk(new_list));
	int heredoc_nbr = count_heredoc(new_list);
	int cmd_blk_nbr = count_cmd_blk(new_list);
	int i = 0;
	while(i < cmd_blk_nbr)
	{
		printf("heredoc_flags[%d] = %d\n",i,mng_hereodocs->heredoc_flags[i]);
		printf("heredoc_fds[%d] = %d\n",i,mng_hereodocs->heredoc_fds[i]);
		i++;
	}
	i = 0;
	while(i < heredoc_nbr)
	{
		printf("heredoc_delims[%d] = %s\n",i,mng_hereodocs->heredoc_delims[i]);
		i++;
	}

	//command_block = parser(*new_list);
	//run_heredoc(command_block); //sadece heredoc varsa, hem heredoc hem dosyalar varsa durumu. //heredoc yoksa ve hatalı dosya varsa bu fonksiyon yakalayamıyor onun için bir fonksiyon yazalım
	//file_cntrl(command_block); //hatalı dosya var ama heredoc hiç yoksa hatalı dosyayı bul ve hatayı bas.//run_heredoc içine koydum birleştirdim
	

/*
int i = 0;

 while (command_block != NULL)
{
    printf("command = %s\n", command_block->command);

    if (command_block->files != NULL)
    {
        i = 0;
        while (command_block->files[i] != NULL)
        {
            printf("files[%d] = %s\n", i, command_block->files[i]);
            i++;
        }
    }
	printf("input fd :%d\n", command_block->input_fd);
	printf("output fd :%d\n", command_block->output_fd);
	if (command_block->heredoc_delimiters != NULL)
	{
		i = 0;
		while (command_block->heredoc_delimiters[i] != NULL)
		{
			printf("heredoc_delimiters[%d] = %s\n", i, command_block->heredoc_delimiters[i]);
			i++;
		}
	}
	if (command_block->heredoc_fd != NULL)
	{
		i = 0;
		while (i < command_block->heredoc_count)
		{
			printf("heredoc_fd[%d] = %d\n", i, command_block->heredoc_fd[i]);
			i++;
		}
	}
    if (command_block->args != NULL)
    {
        i = 0;
        while (command_block->args[i] != NULL)
        {
            printf("args[%d] = %s\n", i, command_block->args[i]);
            i++;
        }
    }
		printf("heredoc count : %d\n",command_block->heredoc_count);
    command_block = command_block->next;
}*/
	return (0);
}