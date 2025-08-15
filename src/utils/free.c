/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:41:51 by falakus           #+#    #+#             */
/*   Updated: 2025/08/15 13:05:31 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_blk(t_command_block *cmd)
{
	t_command_block	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->command)
			free(cmd->command);
		if (cmd->args)
			free_arr(cmd->args);
		if (cmd->fd)
			free(cmd->fd);
		if (cmd->wrong_cmd)
			free(cmd->wrong_cmd);
		free(cmd);
		cmd = tmp;
	}
	cmd = NULL;
}

void	free_lexer(t_lexer_list **lex)
{
	t_lexer_list	*tmp;

	while (*lex)
	{
		tmp = (*lex)->next;
		if ((*lex)->token)
			free((*lex)->token);
		free(*lex);
		*lex = tmp;
	}
	*lex = NULL;
	free(lex);
}

void	free_env(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		if (env_list->line)
			free(env_list->line);
		if (env_list->value)
			free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

void	free_all(t_init	*init)
{
	if (init->lxr_lst)
		free_lexer(init->lxr_lst);
	if (init->cmd_blk && !init->heredoc)
		free_cmd_blk(init->cmd_blk);
	if ((init->jnd_lxr_lst && *(init->jnd_lxr_lst)) || init->exec)
		free_joined_exec(init->jnd_lxr_lst, init->exec);
	if (init->mng_hrdcs && init->mng_hrdcs->f_flag)
		free_mng(init->mng_hrdcs);
	if (init->expnd && init->exit_flag)
		free_expander(init->expnd);
	if (init->env && init->exit_flag)
		free_env(init->env);
	free(init);
	init = NULL;
}
