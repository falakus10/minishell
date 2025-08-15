/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:26:41 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 12:03:22 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init3(t_init *init, t_mng_heredocs *mng)
{
	init->mng_hrdcs = mng;
	init->exit_flag = 0;
	init->mng_hrdcs->f_flag = 1;
}

void	init2(t_init *init, t_joined_lexer_list **jnd_lst \
			, t_command_block *cmd, t_executor *exe)
{
	init->jnd_lxr_lst = jnd_lst;
	init->cmd_blk = cmd;
	init->exec = exe;
}

void	init_structs(t_init *init, t_env **env_list, t_lexer_list **lexer_list)
{
	init->env = env_list;
	init->lxr_lst = lexer_list;
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
