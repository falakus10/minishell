#include "minishell.h"

int g_signal = 0;

void input_loop(char **env, t_tmpv *tmpv)
{
	t_env **env_list;
	t_joined_lexer_list **new_list;
	t_expander *expand;
	t_lexer_list **lexer_list;
	t_init *init;

	init5(&env_list, &expand, env);
	while (1)
	{
		tmpv->flag = 0;
		init = malloc(sizeof(t_init));
		init4(init, expand, &new_list, &lexer_list);
		init_all(init, new_list, env_list, lexer_list);
		tmpv->temp_input = readline("minishell>");
		catch_null(expand, tmpv, init);
		if (input_check(tmpv, init, lexer_list, expand))
			continue;
		free(tmpv->input);
		expand_n_join(lexer_list, env_list, expand, new_list);
		if (heredoc_stuff(tmpv, init, new_list, env_list))
			continue;
		parse_n_exec(init, new_list, env_list, expand);
		free_all(init);
	}
}

int main(int argc, char *argv[], char **env)
{
	(void)argv;
	t_tmpv *tmpv;

	tmpv = malloc(sizeof(t_tmpv));
	tmpv->a = 0;
	tmpv->flag = 0;
	tmpv->input = NULL;
	tmpv->temp_input = NULL;
	if (argc != 1)
		return (0);
	handle_signal();
	input_loop(env, tmpv);
	return (0);
}
