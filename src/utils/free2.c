/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:47:20 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 15:47:21 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_joined_exec(t_joined_lexer_list **jll, t_executor *exe)
{
	t_joined_lexer_list	*tmp;

	while (*jll)
	{
		tmp = (*jll)->next;
		if ((*jll)->token)
			free((*jll)->token);
		(*jll)->token = NULL;
		free(*jll);
		*jll = tmp;
	}
	*jll = NULL;
	free(jll);
	if (exe)
	{
		if (exe->fd)
		{
			free(exe->fd);
			exe->fd = NULL;
		}
		free(exe);
	}
	exe = NULL;
}

void	free_mng(t_mng_heredocs *mng)
{
	if (mng)
	{
		if (mng->heredoc_delims)
			free_arr(mng->heredoc_delims);
		if (mng->heredoc_fds)
			free(mng->heredoc_fds);
		if (mng->heredoc_flags)
			free(mng->heredoc_flags);
		if (mng->heredoc_nums)
			free(mng->heredoc_nums);
		free(mng);
	}
	mng = NULL;
}

void	free_expander(t_expander *exp)
{
	if (exp)
	{
		if (exp->env_key)
			free(exp->env_key);
		if (exp->env_val)
			free(exp->env_val);
		free(exp);
	}
	exp = NULL;
}
