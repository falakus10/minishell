/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_counts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:06:09 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 14:14:54 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**free_heredoc_delimiters(char **delims, int last_index)
{
	while (--last_index >= 0)
		free(delims[last_index]);
	free(delims);
	return (NULL);
}

void	take_heredoc_delims(t_joined_lexer_list **temp, \
		int heredoc_count, t_mng_heredocs **mng_heredocs)
{
	t_joined_lexer_list	*tmp;
	int					i;

	tmp = *temp;
	i = 0;
	while (tmp && i < heredoc_count)
	{
		if (tmp->type == HEREDOC && tmp->next)
		{
			if (tmp->next->type == WORD || tmp->next->type == S_QUOTE \
				|| tmp->next->type == D_QUOTE)
			{
				(*mng_heredocs)->heredoc_delims[i] = \
					ft_strdup(tmp->next->token);
				if (!(*mng_heredocs)->heredoc_delims[i])
					free_heredoc_delimiters((*mng_heredocs)->heredoc_delims, i);
				i++;
			}
		}
		tmp = tmp->next;
	}
	(*mng_heredocs)->heredoc_delims[i] = NULL;
}

void	fill_heredoc_nums(t_mng_heredocs **mng_heredocs, \
	t_joined_lexer_list **temp)
{
	t_joined_lexer_list	*tmp;
	int					i;

	tmp = *temp;
	i = 0;
	while (tmp != NULL)
	{
		if (tmp->type == HEREDOC && tmp->next)
			if (tmp->next->type == WORD || tmp->next->type == D_QUOTE \
				|| tmp->next->type == S_QUOTE)
				(*mng_heredocs)->heredoc_nums[i]++;
		if (tmp->type == PIPE)
			i++;
		tmp = tmp->next;
	}
}

int	count_heredoc(t_joined_lexer_list **temp)
{
	t_joined_lexer_list	*tmp;
	int					heredoc_count;

	tmp = *temp;
	heredoc_count = 0;
	while (tmp != NULL)
	{
		if (tmp->type == HEREDOC && tmp->next)
			if (tmp->next->type == WORD || tmp->next->type == D_QUOTE \
				|| tmp->next->type == S_QUOTE)
				heredoc_count++;
		tmp = tmp->next;
	}
	return (heredoc_count);
}

int	count_cmd_blk(t_joined_lexer_list **temp)
{
	int					pipe_count;
	t_joined_lexer_list	*tmp;

	pipe_count = 0;
	tmp = (*temp);
	while ((tmp) != NULL)
	{
		if (tmp->type == PIPE)
			pipe_count++;
		tmp = tmp->next;
	}
	return (pipe_count + 1);
}
