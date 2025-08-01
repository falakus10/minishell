#include "minishell.h"

void	input_loop(t_command_block *command_block, t_env *env_list, char **env, t_executor *exe)
{
	char *input;
	char *temp_input;
	t_lexer_list **list;
	t_joined_lexer_list **new_list;
	t_expander		*expnd;
	t_mng_heredocs *mng_heredocs;
	expnd = malloc(sizeof(t_expander));
	int flag = 0;
	exe->exp = expnd;

	while (1)
	{
		
		temp_input = readline("minishell>"); //temp_input yerine input kullanamayız çünkü readline'dan dönen alanı kaybederiz, leak çıkar.
		add_history(temp_input);
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
		expander((*list), env_list, expnd);
		remove_quotes(*list);
		new_list = token_join((*list));
		flag = check_tokens(new_list); //tokenlar kontrol edildi
		mng_heredocs = run_hrdcs(new_list,count_cmd_blk(new_list)); //heredoclar işlendi
		if(flag)
			ft_error();
		command_block= parser(*new_list,mng_heredocs);
		executor(command_block, env, exe);
		free(temp_input); // bununla işimiz bitti
		// input'u da işimiz bitince free'lemeliyiz
	}
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
	t_env			**env_list;
	t_executor		*exe;
	t_command_block	*command_block;

	command_block = malloc (sizeof(t_command_block));
	exe = malloc(sizeof(t_executor));
	signal_handler();
	env_list = take_env(env);
	exe->env = *env_list;
	input_loop(command_block, *env_list, env, exe);
	return (0);
}