/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:41:51 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:43:21 by falakus          ###   ########.fr       */
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

void	free_env(t_env **env_list)
{
	t_env	*tmp;
	t_env	*cur;

	if (!env_list || !*env_list)
		return ;
	cur = *env_list;
	while (cur)
	{
		tmp = cur->next;
		if (cur->line)
			free(cur->line);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*env_list = NULL;
	free(env_list);
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
	if (init->env && *init->env && init->exit_flag)
		free_env(init->env);
	free(init);
	init = NULL;
}
