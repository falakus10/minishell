#include "minishell.h"

void init3(t_init *init, t_mng_heredocs *mng)
{
	init->mng_hrdcs = mng;
	init->exit_flag = 0;
}

void init2(t_init *init, t_joined_lexer_list **jnd_lst ,t_command_block *cmd, t_executor *exe)
{
	init->jnd_lxr_lst = jnd_lst;
	init->cmd_blk = cmd;
	init->exec = exe;
}

void init_structs(t_init *init, t_env **env_list, t_lexer_list **lexer_list, t_expander *expander)
{
	init->env = env_list;
	init->lxr_lst = lexer_list;
	init->expnd = expander;
}

void	init_exe(t_executor *exe, t_expander *exp, t_env *envp)
{
	exe->fd = NULL;
	exe->env = envp;
	exe->exp = exp;
}

void	init_expander(t_expander *exp)
{
	exp->env_key = NULL;
	exp->env_val = NULL;
}

void	init_mng_heredocs(t_mng_heredocs *mng, t_env *env_list)
{
	mng->heredoc_delims = NULL;
	mng->heredoc_fds = NULL;
	mng->heredoc_flags = NULL;
	mng->heredoc_nums = NULL;
	mng->env = env_list;
}

void initialize_structs(t_init *init, t_env *env_list)
{
	init_exe(init->exec, init->expnd, env_list);
	init_expander(init->expnd);
	init_mng_heredocs(init->mng_hrdcs, env_list);
}

void	input_loop(char **env)
{
	char *input;
	char *temp_input;
	t_env			**env_list;
	t_joined_lexer_list **new_list;
	t_expander		*expnd;
	t_lexer_list **lexer_list;
	t_init *init;
	int flag;
	//if malloc fail
	env_list = malloc(sizeof(t_env *));
	*env_list = NULL;
	env_list = take_env(env_list, env);
	expnd = malloc(sizeof(t_expander));
	expnd->exit_value = 0;
	
	while (1)
	{
		flag = 0;
		init = malloc(sizeof(t_init));
		init->exec = malloc(sizeof(t_executor));
		init->cmd_blk = NULL;
		init->expnd = expnd;
		init->heredoc = 0;
		init->mng_hrdcs = malloc(sizeof(t_mng_heredocs));
		new_list = malloc(sizeof(t_joined_lexer_list *));
		*new_list = NULL;
		lexer_list = malloc(sizeof(t_lexer_list *));
		*lexer_list = NULL;
		initialize_structs(init,*env_list);
		init_structs(init,env_list,lexer_list,expnd);
		init2(init,new_list,init->cmd_blk,init->exec);
		init3(init,init->mng_hrdcs);
		temp_input = readline("minishell>"); //temp_input yerine input kullanamayız çünkü readline'dan dönen alanı kaybederiz, leak çıkar.
		if (temp_input == NULL)
		{
			init->exit_flag = 1;
			free(temp_input);
			free_all(init);
			write(1, "exit\n", 5);
			exit(0);
		}
		if (temp_input[0] == '\0')
    	{
			free(temp_input);
			//free_all(init);
			free_all(init);
			continue;
    	}	
		input = trim_whitespace(temp_input);
		if (input == NULL || input[0] == '\0')
		{
			free(temp_input);
			free(input);
			free_all(init);
			continue;
		}
		free(temp_input); // bununla işimiz bitti
		add_history(input);
		lexer_function(lexer_list,input);
		free(input);
		expander((*lexer_list), *env_list, expnd);
		remove_quotes(*lexer_list);
		token_join(new_list, *lexer_list);
		flag = check_tokens(new_list,expnd); //tokenlar kontrol edildi
		int a = count_heredoc(new_list);
		if(a != 0)
			init_heredoc_struct(init->mng_hrdcs ,count_cmd_blk(new_list), new_list , *env_list);  //mng_heredocs'a gönderip mng_heredocs a alıyorum bunu deiştir iki değikenli olabilir
		if(a != 0)
		{
			init->heredoc = 1;
			run_hrdcs(init->mng_hrdcs, new_list, init); //heredoclar işlendi
			init->heredoc = 0; //heredoc işlemi bitti
		}
		if(flag)
		{
			free_all(init);
			continue; //exit atıyordum.
		}
		parser(&init->cmd_blk, *new_list, init->mng_hrdcs,expnd); //command_block = parser(command_block, *new_list, mng_heredocs,expnd); durumuna dönülebilir
		executor(init->cmd_blk, init->exec, env_list, init);
		free_all(init);
	}
}

int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	signal_handler();
	input_loop(env);
	return (0);
}