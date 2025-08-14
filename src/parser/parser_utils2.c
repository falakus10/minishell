/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:52:04 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 12:54:02 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pass_cmd_blk(t_command_block **cmd, t_command_block **new,
		t_command_block **tmp)
{
	if ((*cmd) == NULL)
	{
		(*cmd) = (*new);
		(*tmp) = (*cmd);
	}
	else
	{
		(*tmp)->next = (*new);
		(*tmp) = (*tmp)->next;
	}
}

char	*file_path(char *file)
{
	char	buf[256];
	char	*full_path;

	getcwd(buf, sizeof(buf));
	full_path = malloc(ft_strlen(buf) + ft_strlen(file) + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, buf);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, file);
	return (full_path);
}

void	assign_fd(t_command_block **tmp_blk, t_joined_lexer_list **tmp_list,
		t_mng_heredocs *mng)
{
	char	*file_pth;
	int		type;

	type = (*tmp_list)->type;
	handle_ambiguous_redirect(tmp_blk, tmp_list);
	file_pth = file_path((*tmp_list)->next->token);
	if ((*tmp_blk)->file_err == 0)
	{
		if (type == REDIR_IN)
			handle_input_redirection(tmp_blk, tmp_list, file_pth, mng);
		else if (type == REDIR_OUT)
			handle_output_redirection(tmp_blk, tmp_list, file_pth);
		else if (type == APPEND)
			handle_append_redirection(tmp_blk, tmp_list, file_pth);
	}
	free(file_pth);
}

void	handle_redirect_token(t_joined_lexer_list **temp,
		t_command_block **temp_block, t_mng_heredocs *mng_heredocs)
{
	if ((*temp)->type == HEREDOC)
	{
		if (mng_heredocs->heredoc_flags[mng_heredocs->index])
			(*temp_block)->input_fd = \
			mng_heredocs->heredoc_fds[mng_heredocs->index];
		*temp = (*temp)->next;
		return ;
	}
	assign_fd((temp_block), (temp), mng_heredocs);
	if (mng_heredocs->heredoc_flags)
	{
		if (mng_heredocs->heredoc_flags[mng_heredocs->index])
			(*temp_block)->input_fd = \
			mng_heredocs->heredoc_fds[mng_heredocs->index];
	}
	*temp = (*temp)->next;
}

void	handle_token_logic(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		int *is_cmd_pointed, t_mng_heredocs *mng_heredocs)
{
	if ((*tmp)->token[0] == '\0' && (*tmp)->next != NULL)
		(*tmp) = (*tmp)->next;
	if ((*tmp)->next != NULL && is_redirect_type((*tmp)->type))
	{
		handle_redirect_token(tmp, tmp_blk, mng_heredocs);
	}
	else if (is_word_type((*tmp)->type))
	{
		if (!(*is_cmd_pointed))
		{
			if (handle_command_token(tmp, tmp_blk, is_cmd_pointed))
				return ;
		}
		else
			handle_argument_token(tmp, tmp_blk);
	}
	(*tmp) = (*tmp)->next;
}
