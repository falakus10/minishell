#include "minishell.h"

void	input_loop(t_command_block *command_block, t_env *env_list, t_executor *exe)
{
	char *input;
	char *temp_input;
	t_lexer_list **list;
	t_joined_lexer_list **new_list;
	t_expander		*expnd;
	t_mng_heredocs *mng_heredocs;
	expnd = malloc(sizeof(t_expander));
	expnd->exit_value = 0;
	int flag = 0;
	exe->exp = expnd;

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
		add_history(input);
		list = lexer_function(input);
		expander((*list), env_list, expnd);
		remove_quotes(*list);
		new_list = token_join(*list);
		flag = check_tokens(new_list,expnd); //tokenlar kontrol edildi
		int a = count_heredoc(new_list);
		mng_heredocs = init_heredoc_struct(count_cmd_blk(new_list), new_list);
		if(a != 0)
			mng_heredocs = run_hrdcs(mng_heredocs, new_list); //heredoclar işlendi
		if(flag)
		{
			//free
			continue; //exit atıyordum.
		}	
		command_block= parser(*new_list,mng_heredocs,expnd,env_list);
		executor(command_block, env_list, exe);
		free(temp_input); // bununla işimiz bitti
		// input'u da işimiz bitince free'lemeliyiz
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
	input_loop(command_block, *env_list, exe);
	return (0);
}