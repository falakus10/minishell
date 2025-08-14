/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:56:17 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 12:56:19 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_null(t_joined_lexer_list **tmp)
{
	if ((*tmp) != NULL)
		(*tmp) = (*tmp)->next;
}

void	handle_errors(t_command_block *temp_block)
{
	if (temp_block->file_err == 0 && temp_block->cmd_err == 1)
	{
		write(2, temp_block->wrong_cmd, ft_strlen(temp_block->wrong_cmd));
		write(2, ": command not found\n", 20);
		temp_block->last_fault = 1;
		temp_block->expnd->exit_value = 127;
	}
	else if (temp_block->path_err == 1 && temp_block->file_err == 0)
	{
		write(2, "bash: ", 6);
		write(2, temp_block->wrong_cmd, ft_strlen(temp_block->wrong_cmd));
		write(2, ": No such file or directory\n", 28);
		temp_block->expnd->exit_value = 127;
	}
	else if (temp_block->file_err == 0 && temp_block->command == NULL)
	{
		temp_block->file_err = 1;
	}
}

void	parser(t_command_block **command_block, t_joined_lexer_list *list,
		t_mng_heredocs *mng_heredocs, t_expander *expander)
{
	t_command_block		*new_block;
	t_command_block		*temp_block;
	t_joined_lexer_list	*temp;
	int					is_cmd_pointed;

	temp_block = NULL;
	temp = list;
	new_block = NULL;
	if (temp == NULL)
		return ;
	while (temp != NULL)
	{
		is_cmd_pointed = 0;
		new_block = init_command_block(expander, mng_heredocs->env);
		pass_cmd_blk(command_block, &new_block, &temp_block);
		while (temp != NULL && temp->type != PIPE)
			handle_token_logic(&temp, &temp_block, &is_cmd_pointed,
				mng_heredocs);
		handle_errors(temp_block);
		mng_heredocs->index++;
		check_null(&temp);
	}
}
