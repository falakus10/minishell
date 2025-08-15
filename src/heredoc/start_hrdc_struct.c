/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_hrdc_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:19:54 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 13:05:19 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_heredoc_struct(t_mng_heredocs *mng, int count,
	t_joined_lexer_list **temp, t_env *env_list)
{
	mng->index = 0;
	mng->heredoc_flags = malloc(sizeof(int) * count);
	mng->heredoc_fds = malloc(sizeof(int) * count);
	mng->heredoc_nums = malloc(sizeof(int) * count);
	mng->heredoc_delims = malloc(sizeof(char *) * (count_heredoc(temp) + 1));
	mng->env = env_list;
	if (!mng->heredoc_flags || !mng->heredoc_fds \
		|| !mng->heredoc_nums || !mng->heredoc_delims)
		return ;
	fill_int_array(mng->heredoc_flags, 0, count);
	fill_int_array(mng->heredoc_fds, -3, count);
	fill_int_array(mng->heredoc_nums, 0, count);
}

void	check_redirin(t_joined_lexer_list *scan, int *heredoc_valid)
{
	t_joined_lexer_list	*tmp;

	tmp = scan;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN)
		{
			*heredoc_valid = 0;
			break ;
		}
		tmp = tmp->next;
	}
}

void	fill_heredoc_flags(t_mng_heredocs *mng, t_joined_lexer_list **temp)
{
	t_joined_lexer_list	*tmp;
	int					i;
	t_joined_lexer_list	*scan;
	int					heredoc_valid;

	tmp = *temp;
	i = 0;
	heredoc_valid = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next)
		{
			if (tmp->next->type == WORD || tmp->next->type == D_QUOTE \
				|| tmp->next->type == S_QUOTE)
				heredoc_valid = 1;
			scan = tmp->next->next;
			check_redirin(scan, &heredoc_valid);
			if (heredoc_valid)
				mng->heredoc_flags[i] = 1;
		}
		if (tmp->type == PIPE)
			i++;
		tmp = tmp->next;
	}
}

int	run_hrdcs(t_mng_heredocs *mng, t_joined_lexer_list **temp, t_init *init)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	fill_heredoc_flags(mng, temp);
	take_heredoc_delims(temp, count_heredoc(temp), &mng);
	fill_heredoc_nums(&mng, temp);
	if (heredoc_handle(mng, i, j, init))
	{
		init->heredoc = 0;
		return (1);
	}
	init->heredoc = 0;
	return (0);
}
