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


int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	t_lexer_list	*temp;
	t_lexer_list	**list;
	t_env			**env_list;
	t_executor		*exe;
	t_expander		*exp;
	t_command_block	*command_block;
	t_joined_lexer_list **new_list;
	t_mng_heredocs *mng_heredocs;
	int flag;
	exp = malloc(sizeof(t_expander));
	exe = malloc(sizeof(t_executor));
	if (!exp)
		return (0);
	signal_handler();
	list = input_loop(); //bu listede lexer'da ayrılmış olan token'ları tutuyoruz 
	temp = *list;	
	env_list = take_env(env);
	exe->env = *env_list;
 	expander(temp, *env_list, exp);
	remove_quotes(*list);
	new_list = token_join(temp);
	flag = check_tokens(new_list); //tokenlar kontrol edildi
	mng_heredocs = run_hrdcs(new_list,count_cmd_blk(new_list)); //heredoclar işlendi
	if(flag)
		ft_error();
	command_block= parser(*new_list,mng_heredocs);
	executor(command_block, env, exe);
/* 	int i = 0;
	
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
		if (command_block->args != NULL)
		{
			i = 0;
			while (command_block->args[i] != NULL)
			{
				printf("args[%d] = %s\n", i, command_block->args[i]);
				i++;
			}
		}
		command_block = command_block->next;
	}
 */

	
	
	/*
	int i = 0;
	while(i < count_cmd_blk(new_list))
	{
		printf("heredoc_flags[%d] = %d\n",i,mng_heredocs->heredoc_flags[i]);
		printf("heredoc_fds[%d] = %d\n",i,mng_heredocs->heredoc_fds[i]);
		printf("heredoc_nums[%d] = %d\n",i,mng_heredocs->heredoc_nums[i]);
		i++;
	}*/
	
	//run_heredoc(command_block); //sadece heredoc varsa, hem heredoc hem dosyalar varsa durumu. //heredoc yoksa ve hatalı dosya varsa bu fonksiyon yakalayamıyor onun için bir fonksiyon yazalım
	//file_cntrl(command_block); //hatalı dosya var ama heredoc hiç yoksa hatalı dosyayı bul ve hatayı bas.//run_heredoc içine koydum birleştirdim
	
	
	


	return (0);
}