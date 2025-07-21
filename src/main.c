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
}

void run_heredoc(t_command_block *tmp_blk) //kalıcı değişiklik yapacak mıyım ?
{
	int heredoc_fd[(tmp_blk)->heredoc_count][2];

		if ((tmp_blk->err_flg) == -2) //hiç hatalı dosya yoksa sadece heredocları çalıştır
		{
			while(tmp_blk != NULL)
			{
				heredoc_handle((tmp_blk)->heredoc_count,heredoc_fd,(tmp_blk)->heredoc_delimiters,(tmp_blk)->lst_typ);
				tmp_blk = tmp_blk->next;
			}
		}
		else // hatalı dosya varsa önce heredocları çalıştır sonra hata bas
		{
			while(tmp_blk != NULL)
			{
				heredoc_handle((tmp_blk)->heredoc_count,heredoc_fd,(tmp_blk)->heredoc_delimiters,(tmp_blk)->lst_typ);
				tmp_blk = tmp_blk->next;
			}
			printf("bash: no such file or directory%s\n",tmp_blk->files[tmp_blk->err_flg]);
			ft_error();//exit yapıyor
		}
	
}


int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	t_lexer_list	*temp;
	t_lexer_list	**list;
	t_expander		*exp;
	t_command_block	*command_block;
	t_joined_lexer_list **new_list;

	exp = malloc(sizeof(t_expander));
	if (!exp)
		return (0);
	signal_handler();
	list = input_loop(); //bu listede lexer'da ayrılmış olan token'ları tutuyoruz 
	temp = *list;
	expander(temp, env, exp);
	remove_quotes(*list);
	new_list = token_join(temp);
	command_block = parser(*new_list);
	run_heredoc(command_block);



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