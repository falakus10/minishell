/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:57:03 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 16:06:22 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_type(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT \
		|| type == APPEND || type == HEREDOC);
}

int	is_word_type(int type)
{
	return (type == WORD || type == S_QUOTE || type == D_QUOTE);
}

void	set_command_if_valid(t_command_block **tmp_blk, char *token)
{
	if (is_builtin(token))
		(*tmp_blk)->command = ft_strdup(token);
	else
	{
		if (!create_path((*tmp_blk), token) && (*tmp_blk)->cmd_err == 0)
		{
			(*tmp_blk)->command = ft_strdup(token);
			(*tmp_blk)->wrong_cmd = ft_strdup(token);
			(*tmp_blk)->cmd_err = 1;
		}
	}
}

int	handle_command_token(t_joined_lexer_list **tmp, \
	t_command_block **tmp_blk, int *is_cmd_pointed)
{
	if ((*tmp)->token[0] == '\0')
	{
		(*tmp) = (*tmp)->next;
		return (1);
	}
	set_command_if_valid(tmp_blk, (*tmp)->token);
	(*tmp_blk)->args = append_to_array((*tmp_blk)->args, \
		(*tmp_blk)->argument_count, (*tmp)->token);
	(*tmp_blk)->argument_count++;
	*is_cmd_pointed = 1;
	return (0);
}

void	handle_argument_token(t_joined_lexer_list **tmp, \
			t_command_block **tmp_blk)
{
	(*tmp_blk)->args = append_to_array((*tmp_blk)->args, \
		(*tmp_blk)->argument_count, (*tmp)->token);
	(*tmp_blk)->argument_count++;
}
