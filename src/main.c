#include "minishell.h"

/*
malloclananlar

env_list :ok :!!take_env'ye env_list 'i gönderip tekrar env_list'e atıyorum doğru olmayabilir ona bak
exe :ok
command_block :ok :!!parser'a command block atılıp return'u de command blocka atılıyor ona bak, bunlar void olabilir
expnd :ok
lexer_list	:ok
mng_heredocs :ok :!!mng_heredocs'a gönderip mng_heredocs a alıyorum bunu deiştir iki değikenli olabilir
joined_lexer_list :ok

içerden sildiysem ok verdim, void yapıcam
*/


void init3(t_init *init, t_mng_heredocs *mng)
{
	init->mng_hrdcs = mng;
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

void init_cmd_blk(t_command_block *cmd, t_env *env_list, t_expander *exp)
{
	cmd->next = NULL;
	cmd->args = NULL;
	cmd->fd = NULL;
	cmd->last_fault = 0;
	cmd->files = NULL;
	cmd->operators = NULL;
	cmd->heredoc_fd = NULL;
	cmd->heredoc_count = 0;
	cmd->operator_count = 0;
	cmd->argument_count = 0;
	cmd->cmd_count = 0;
	cmd->input_fd = -3;//öylesine -3 ile başlattım önemli mi ?
	cmd->output_fd = -3;
	cmd->err_flg = -2; //sanırım kullanılmıyor //öylesine -2 ile başlattım önemli mi ?
	cmd->err_sign = 0; // sanırım kullanılmıyor
	cmd->file_err = 0;
	cmd->cmd_err = 0;
	cmd->command = NULL;
	cmd->wrong_cmd = NULL;
	cmd->expnd = exp;
	cmd->path_err = 0;
	cmd->wrong_path = 0;
	cmd->env = env_list;
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
	init_cmd_blk(init->cmd_blk,env_list, init->expnd);
	//init_lexer(init->lxr_lst);
	//init_jnd_lexer(init->jnd_lxr_lst);
	init_exe(init->exec, init->expnd, env_list);
	init_expander(init->expnd);
	init_mng_heredocs(init->mng_hrdcs, env_list);
}

void	input_loop(char **env)
{
	char *input;
	char *temp_input;
	t_env			**env_list;
	t_executor		*exe;
	t_command_block	*command_block;
	t_joined_lexer_list **new_list;
	t_expander		*expnd;
	t_mng_heredocs *mng_heredocs;
	t_lexer_list **lexer_list;
	t_init *init;
	//if malloc fail
	
	while (1)
	{
		init = malloc(sizeof(t_init));
		env_list = malloc(sizeof(t_env *));
		*env_list = NULL;
		exe = malloc(sizeof(t_executor));
		command_block = malloc (sizeof(t_command_block));
		env_list = take_env(env_list, env);
		exe->env = *env_list;
		new_list = malloc(sizeof(t_joined_lexer_list *));
		*new_list = NULL;
		expnd = malloc(sizeof(t_expander));
		mng_heredocs = malloc(sizeof(t_mng_heredocs));
		lexer_list = malloc(sizeof(t_lexer_list *));
		*lexer_list = NULL;
		
		expnd->exit_value = 0;
		int flag = 0;
		//exe->exp = expnd;
		init_structs(init,env_list,lexer_list,expnd);
		init2(init,new_list,command_block,exe);
		init3(init,mng_heredocs);
		initialize_structs(init,*env_list);
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
			init_heredoc_struct(mng_heredocs ,count_cmd_blk(new_list), new_list , *env_list);  //mng_heredocs'a gönderip mng_heredocs a alıyorum bunu deiştir iki değikenli olabilir
		if(a != 0)
			run_hrdcs(mng_heredocs, new_list); //heredoclar işlendi
		if(flag)
		{
			free_all(init, env_list);
			continue; //exit atıyordum.
		}
		parser(&command_block, *new_list, mng_heredocs,expnd); //command_block = parser(command_block, *new_list, mng_heredocs,expnd); durumuna dönülebilir
		executor(command_block, exe, env_list);
		//free_all(init, env_list);
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