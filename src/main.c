#include "minishell.h"


int g_signal = 0;

void init3(t_init *init, t_mng_heredocs *mng)
{
	init->mng_hrdcs = mng;
	init->exit_flag = 0;
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

void initialize_structs(t_init *init, t_env *env_list, t_expander *exp)
{
	init->env = &env_list;
	init->exit_flag = 0;
	init->heredoc = 0;
	init->expnd = exp;
	init->cmd_blk = NULL;
	init->mng_hrdcs = malloc(sizeof(t_mng_heredocs));
	init->mng_hrdcs->f_flag = 1;
	init->jnd_lxr_lst = malloc(sizeof(t_joined_lexer_list *));
	*(init->jnd_lxr_lst) = NULL;
	init->lxr_lst = malloc(sizeof(t_lexer_list *));
	*(init->lxr_lst) = NULL;
	init->exec = malloc(sizeof(t_executor));
	init_exe(init->exec, init->expnd, env_list);
	init_expander(init->expnd);
	init_mng_heredocs(init->mng_hrdcs, env_list);
}

void	input_loop(char **env)
{
	char *input;
	char *temp_input;
	t_init *init;
	t_env			**env_list;
	t_expander		*expand;
	int flag;

	env_list = malloc(sizeof(t_env *));
	*env_list = NULL;
	take_env(env_list, env);
	expand = malloc(sizeof(t_expander));
	expand->exit_value = 0;
	g_signal = 0;
	
	while (1)
	{
		handle_signal();
		flag = 0;
		init = malloc(sizeof(t_init));
		initialize_structs(init, *env_list, expand);
		temp_input = readline("minishell>");
		if (g_signal == 130)
		{
			expand->exit_value = 130;
		}
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
		if(lexer_function(init->lxr_lst,input) == -1)
		{
			write(2, "bash : Unclosed quotes\n", 23);
			expand->exit_value = 2;
			free(input);
			free_all(init);
			continue;
		}
		free(input);
		expander(*(init->lxr_lst), *env_list, expand);
		remove_quotes(*(init->lxr_lst));
		token_join(init->jnd_lxr_lst, *(init->lxr_lst));
		flag = check_tokens(init->jnd_lxr_lst, expand);
		int a = count_heredoc(init->jnd_lxr_lst);
		if(a != 0)
			init_heredoc_struct(init->mng_hrdcs ,count_cmd_blk(init->jnd_lxr_lst), init->jnd_lxr_lst , *env_list);
		if(a != 0)
		{
			init->heredoc = 1;
			if(run_hrdcs(init->mng_hrdcs, init->jnd_lxr_lst, init))
			{
				init->heredoc = 0;
				expand->exit_value = 130;
				free_all(init);
				continue;
			}
			init->heredoc = 0;
		}
		if(flag)
		{
			free_all(init);
			continue;
		}
		parser(&init->cmd_blk, *(init->jnd_lxr_lst), init->mng_hrdcs, expand);
		executor(init->cmd_blk, init->exec, env_list, init);
		free_all(init);
	}
}

int	main(int argc, char *argv[], char **env)
{
	(void)argv;
	if (argc != 1)
		return (0);
	input_loop(env);
	return (0);
}