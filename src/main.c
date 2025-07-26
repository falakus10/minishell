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


void run_heredoc(t_command_block *tmp_blk) //kalıcı değişiklik yapacak mıyım ?
{
	int heredoc_fd[(tmp_blk)->heredoc_count][2];
	t_command_block *iter = tmp_blk; // orijinal başlangıcı sakla
	t_command_block *last_error_block = NULL;
	int err_count = 0;
	while (iter != NULL)
	{
		if (iter->err_flg != -2)
		{
			last_error_block = iter;
			err_count++;
		}
		iter = iter->next;
	}
	iter = tmp_blk;
	while(tmp_blk != NULL) //her komut bloğununn heredoc count'u ayrı tutuluyor ama her komut bloğunu gezdiğim için hepsini işleyebilmiş oluyorum
	{
		heredoc_handle((tmp_blk)->heredoc_count,heredoc_fd,(tmp_blk)->heredoc_delimiters,(tmp_blk)->lst_typ);
		tmp_blk = tmp_blk->next;
	}
	if(err_count != 0)
		file_cntrl(iter,last_error_block);
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

	run_heredoc(command_block); //sadece heredoc varsa, hem heredoc hem dosyalar varsa durumu. //heredoc yoksa ve hatalı dosya varsa bu fonksiyon yakalayamıyor onun için bir fonksiyon yazalım
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