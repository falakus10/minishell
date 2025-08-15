/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:26:12 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 14:12:27 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	catch_null(t_expander *expand, t_tmpv *tmpv, t_init *init)
{
	if (g_signal == 130)
		expand->exit_value = 130;
	if (tmpv->temp_input == NULL)
	{
		init->exit_flag = 1;
		free(tmpv->temp_input);
		free_all(init);
		write(1, "exit\n", 5);
		exit(0);
	}
}

void	init_env_expand(t_env ***env_list, t_expander **expand, char **env)
{
	*env_list = malloc(sizeof(t_env *));
	**env_list = NULL;
	*env_list = take_env(*env_list, env);
	*expand = malloc(sizeof(t_expander));
	(**expand).exit_value = 0;
}

void	expand_n_join(t_lexer_list **lexer_list, t_env **env_list, \
			t_expander *expand, t_joined_lexer_list **new_list)
{
	expander((*lexer_list), *env_list, expand);
	remove_quotes(*lexer_list);
	token_join(new_list, *lexer_list);
}

void	parse_n_exec(t_init *init, t_joined_lexer_list **new_list, \
			t_env **env_list, t_expander *expand)
{
	parser(&init->cmd_blk, *new_list, init->mng_hrdcs, expand);
	executor(init->cmd_blk, init->exec, env_list, init);
	init->mng_hrdcs->f_flag = 1;
}

void	init_all(t_init *init, t_joined_lexer_list **new_list,
			t_env **env_list, t_lexer_list **lexer_list)
{
	initialize_structs(init, *env_list);
	init_structs(init, env_list, lexer_list);
	init2(init, new_list, init->cmd_blk, init->exec);
	init3(init, init->mng_hrdcs);
}
