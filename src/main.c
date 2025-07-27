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


void heredoc_handle(int heredoc_count,int heredoc_fd[][2],char **heredoc_delims,int lst_type)
{
	int		i;
	pid_t	pid;
	char	*line;

	for (i = 0; i < heredoc_count; i++)
	{
		if (pipe(heredoc_fd[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) // child
		{
			close(heredoc_fd[i][0]); // read ucunu kapat
			while (1)
			{
				line = readline("> ");
				if (!line || strcmp(line, heredoc_delims[i]) == 0)
				{
					free(line);
					break;
				}
				write(heredoc_fd[i][1], line, strlen(line));
				write(heredoc_fd[i][1], "\n", 1);
				free(line);
			}
			close(heredoc_fd[i][1]);
			exit(0);
		}
		else // parent
		{
			close(heredoc_fd[i][1]); // write ucunu kapat
			waitpid(pid, NULL, 0);
		}
	}

	// İş bittikten sonra: son heredoc komutun input'u olacak mı?
	for (i = 0; i < heredoc_count; i++)
	{
		if (i == heredoc_count - 1 && lst_type)
			continue; // bu fd'yi komut alacak, açık bırak
		close(heredoc_fd[i][0]); // Diğer heredoc'ların read uçlarını kapat
	}

	/*
	// FD'leri kapat
	for (i = 0; i < heredoc_count; i++)
	{
		if (i == heredoc_count - 1 && lst_type)  // pipedan önceki veya son komut bloğunun sonundaki eleman << delim ise input_fdye heredocun fdsini ata
			*input_fd = heredoc_fd[i][0]; // son heredoc komutun input'u olacak
		else
			close(heredoc_fd[i][0]); // diğerlerini kapat
	}
	*/
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
/* 
int count_pipes(t_joined_lexer_list **temp)
{
	int pipe_count;

	pipe_count = 0;
	while((*temp) != NULL)
	{
		if((*temp)->type == HEREDOC)
	}
}

void run_heredoc(t_joined_lexer_list **temp) 
{
	
} */

void print_error_check(t_joined_lexer_list *tmp)
{
	if(tmp->next->type == REDIR_IN)
	{
		printf("bash: syntax error near unexpected token `<'\n");
		ft_error();
	}
	else if(tmp->next->type == REDIR_OUT)
	{
		printf("bash: syntax error near unexpected token `>'\n");
		ft_error();
	}
	else if(tmp->next->type == APPEND)
	{
		printf("bash: syntax error near unexpected token `>>'\n");
		ft_error();
	}
	else if(tmp->next->type == HEREDOC)
	{
		printf("bash: syntax error near unexpected token `<<'\n");
		ft_error();
	}
	else if(tmp->next->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		ft_error();
	}
}


void check_tokens(t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	
	tmp = *temp;
	if((tmp != NULL) && (tmp)->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		ft_error();
	}
	while(tmp != NULL)
	{	
		if((tmp->type >= 2 && tmp->type <=5) && (tmp->next == NULL || tmp->next->type == PIPE)) //operatörler tek başına olamaz
		{
			if(tmp->next == NULL)
			{
				printf("bash: syntax error near unexpected token `newline'\n");
				ft_error();
			}
			else if(tmp->next->type == PIPE)
			{	
				printf("bash: syntax error near unexpected token `|'");
				ft_error();
			}
		}
		while (tmp->type != PIPE && tmp->next != NULL)
		{
			if((tmp->type >= 2 && tmp->type <=5))
			{
				print_error_check(tmp);
			}
		tmp = tmp->next;
		}
	tmp = tmp->next;
	}	
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

	exp = malloc(sizeof(t_expander));
	if (!exp)
		return (0);
	signal_handler();
	list = input_loop(); //bu listede lexer'da ayrılmış olan token'ları tutuyoruz 
	temp = *list;
	env_list = take_env(env);
	//tmp = *env_list;

 	expander(temp, *env_list, exp);
	remove_quotes(*list);
	new_list = token_join(temp);
	check_tokens(new_list);
	//run_heredoc()


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