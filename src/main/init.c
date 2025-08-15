/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 12:50:51 by falakus           #+#    #+#             */
/*   Updated: 2025/08/15 13:03:23 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init3(t_init *init, t_mng_heredocs *mng)
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

void	initalize_structs(t_init *init, t_env *env_list, t_expander *exp)
{
	init->env = env_list;
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
