/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 12:10:46 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 13:44:50 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	input_loop(char **env, t_tmpv *tmpv)
{
	t_env				**env_list;
	t_joined_lexer_list	**new_list;
	t_expander			*expand;
	t_lexer_list		**lexer_list;
	t_init				*init;

	init_env_expand(&env_list, &expand, env);
	while (1)
	{
		tmpv->tkn_syntax = 0;
		init = malloc(sizeof(t_init));
		allocate_n_assign(init, expand, &new_list, &lexer_list);
		init_all(init, new_list, env_list, lexer_list);
		tmpv->temp_input = readline("minishell>");
		catch_null(expand, tmpv, init);
		if (input_check(tmpv, init, lexer_list, expand))
			continue ;
		free(tmpv->input);
		expand_n_join(lexer_list, env_list, expand, new_list);
		if (check_syntax_n_heredoc(tmpv, init, new_list, env_list))
			continue ;
		parse_n_exec(init, new_list, env_list, expand);
		free_all(init);
	}
}

int	main(int argc, char *argv[], char **env)
{
	t_tmpv	*tmpv;

	(void)argv;
	tmpv = malloc(sizeof(t_tmpv));
	tmpv->heredoc_count = 0;
	tmpv->tkn_syntax = 0;
	tmpv->input = NULL;
	tmpv->temp_input = NULL;
	if (argc != 1)
		return (0);
	handle_signal();
	input_loop(env, tmpv);
	return (0);
}
