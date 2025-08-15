/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 12:50:47 by falakus           #+#    #+#             */
/*   Updated: 2025/08/15 13:19:33 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	input_run(t_init *init, t_expander *expand, t_env *env_list, int a)
{
	int	flag;

	expander(*(init->lxr_lst), env_list, expand);
	remove_quotes(*(init->lxr_lst));
	token_join(init->jnd_lxr_lst, *(init->lxr_lst));
	flag = check_tokens(init->jnd_lxr_lst, expand);
	a = count_heredoc(init->jnd_lxr_lst);
	if (a != 0)
	{
		init_heredoc_struct(init->mng_hrdcs, count_cmd_blk(init->jnd_lxr_lst),
			init->jnd_lxr_lst, env_list);
		init->heredoc = 1;
		if (run_hrdcs(init->mng_hrdcs, init->jnd_lxr_lst, init))
		{
			expand->exit_value = 130;
			free_all(init);
			return (1);
		}
	}
	if (flag)
	{
		free_all(init);
		return (1);
	}
	return (0);
}

int	input_check(char *temp_input, char *input, t_init *init, t_expander *expand)
{
	if (temp_input[0] == '\0')
	{
		free(temp_input);
		free_all(init);
		return (1);
	}
	input = trim_whitespace(temp_input);
	free(temp_input);
	if (input == NULL || input[0] == '\0')
	{
		free(input);
		free_all(init);
		return (1);
	}
	add_history(input);
	if (lexer_function(init->lxr_lst, input) == -1)
	{
		write(2, "bash : Unclosed quotes\n", 23);
		expand->exit_value = 2;
		free(input);
		free_all(init);
		return (1);
	}
	free(input);
	return (0);
}

char	*get_line(t_init *init, t_env *env_list, t_expander *expand)
{
	char	*temp_input;

	initalize_structs(init, env_list, expand);
	temp_input = readline("minishell>");
	if (g_signal == 130)
		expand->exit_value = 130;
	if (temp_input == NULL)
	{
		init->exit_flag = 1;
		free(temp_input);
		free_all(init);
		write(1, "exit\n", 5);
		exit(0);
	}
	return (temp_input);
}

void	input_loop(char **env, int a)
{
	char		*input;
	char		*temp_input;
	t_init		*init;
	t_env		*env_list;
	t_expander	*expand;

	input = NULL;
	env_list = take_env(env);
	expand = malloc(sizeof(t_expander));
	expand->exit_value = 0;
	g_signal = 0;
	while (1)
	{
		handle_signal();
		init = malloc(sizeof(t_init));
		temp_input = get_line(init, env_list, expand);
		if (input_check(temp_input, input, init, expand))
			continue ;
		if (input_run(init, expand, env_list, a))
			continue ;
		parser(&init->cmd_blk, *(init->jnd_lxr_lst), init->mng_hrdcs, expand);
		executor(init->cmd_blk, init->exec, env_list, init);
		free_all(init);
	}
}

int	main(int argc, char *argv[], char **env)
{
	int	a;

	(void)argv;
	a = 0;
	if (argc != 1)
		return (0);
	input_loop(env, a);
	return (0);
}
