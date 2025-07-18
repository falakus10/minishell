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

void	first_pipe_ctrl(t_joined_lexer_list *temp)
{
	if (temp->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		ft_error();
	}
}

void	handle_redirect_token(t_joined_lexer_list **temp,
		t_command_block **temp_block)
{
	if ((*temp)->next->type != WORD && (*temp)->next->type != S_QUOTE
		&& (*temp)->next->type != D_QUOTE)
	{
		printf("Syntax error: Operator must be followed by a word\n");
		ft_error();
	}
	if ((*temp)->type == HEREDOC)
	{
		(*temp_block)->heredoc_delimiters = append_to_array((*temp_block)->heredoc_delimiters,
				(*temp_block)->heredoc_count, (*temp)->next->token);
		(*temp_block)->heredoc_count++;
		*temp = (*temp)->next;
	}
	else
	{
		(*temp_block)->files = append_to_array((*temp_block)->files,
				(*temp_block)->operator_count, (*temp)->next->token);
		(*temp_block)->operator_count++;
		*temp = (*temp)->next;
	}
}

void	handle_token_logic(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils)
{
	if ((*tmp)->next != NULL && ((*tmp)->type == REDIR_IN
			|| (*tmp)->type == REDIR_OUT || (*tmp)->type == APPEND
			|| (*tmp)->type == HEREDOC))
		handle_redirect_token(tmp, tmp_blk);
	else if (!(utils->is_cmd_pointed) && ((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		(*tmp_blk)->command = ft_strdup((*tmp)->token);
		utils->is_cmd_pointed = 1;
	}
	else if (utils->is_cmd_pointed && ((*tmp)->type == WORD
			|| (*tmp)->type == S_QUOTE || (*tmp)->type == D_QUOTE))
	{
		(*tmp_blk)->args = append_to_array((*tmp_blk)->args,
				(*tmp_blk)->argument_count, (*tmp)->token);
		(*tmp_blk)->argument_count++;
	}
	utils->last_token_type = (*tmp)->type;
	(*tmp) = (*tmp)->next;
}

void	first_tkn_chck(t_pipeline_utils *utils, t_joined_lexer_list *temp)
{
	if (utils->first_token_flg == 0 && temp->next == NULL
		&& (temp->type == REDIR_IN || temp->type == REDIR_OUT
			|| temp->type == APPEND || temp->type == HEREDOC))
	{
		printf("Syntax error: Operator cannot be at the end\n");
		ft_error();
	}
}