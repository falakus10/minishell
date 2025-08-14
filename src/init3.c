/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:07:43 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 21:09:58 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_mng_heredocs(t_mng_heredocs *mng, t_env *env_list)
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

void init4(t_init *init, t_expander *expand, t_joined_lexer_list ***new_list, t_lexer_list ***lexer_list)
{
	init->exec = malloc(sizeof(t_executor));
	init->cmd_blk = NULL;
	init->expnd = expand;
	init->heredoc = 0;
	init->mng_hrdcs = malloc(sizeof(t_mng_heredocs));
	*new_list = malloc(sizeof(t_joined_lexer_list *));
	**new_list = NULL;
	*lexer_list = malloc(sizeof(t_lexer_list *));
	**lexer_list = NULL;
}

int input_check(t_tmpv *tmpv, t_init *init, t_lexer_list **lxr, t_expander *expnd)
{
	if (tmpv->temp_input[0] == '\0')
	{
		free(tmpv->temp_input);
		free_all(init);
		return (1);
	}
	tmpv->input = trim_whitespace(tmpv->temp_input);
	if (tmpv->input == NULL || tmpv->input[0] == '\0')
	{
		free(tmpv->temp_input);
		free(tmpv->input);
		free_all(init);
		return (1);
	}
	free(tmpv->temp_input);
	add_history(tmpv->input);
	if (lexer_function(lxr, tmpv->input) == -1)
	{
		write(2, "bash : Unclosed quotes\n", 23);
		expnd->exit_value = 2;
		free(tmpv->input);
		free_all(init);
		return (1);
	}
	return (0);
}

int heredoc_stuff(t_tmpv *tmpv, t_init *init, t_joined_lexer_list **new_list, t_env **env_list)
{
	tmpv->flag = check_tokens(new_list, init->expnd);
	tmpv->a = count_heredoc(new_list);
	if (tmpv->a != 0)
		init_heredoc_struct(init->mng_hrdcs, count_cmd_blk(new_list), new_list, *env_list);
	if (tmpv->a != 0)
	{
		init->heredoc = 1;
		if (run_hrdcs(init->mng_hrdcs, new_list, init))
		{
			init->heredoc = 0;
			init->expnd->exit_value = 130;
			free_all(init);
			return (1);
		}
		init->heredoc = 0;
	}
	if (tmpv->flag)
	{
		free_all(init);
		return (1);
	}
	return (0);
}