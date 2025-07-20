#include "minishell.h"

void	loop(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils)
{
	first_tkn_chck(utils, *tmp);
	utils->first_token_flg = 1;
	handle_token_logic(tmp, tmp_blk, utils);
}

void	pipeline_available(t_joined_lexer_list *temp, t_pipeline_utils *utils)
{
	if (temp != NULL && temp->type == PIPE && temp->next == NULL)
	{
		printf("Syntax error: input cannot end with a pipe '|'\n");
		ft_error();
	}
	if (temp == NULL && (utils->last_token_type == PIPE
			|| utils->last_token_type == REDIR_IN
			|| utils->last_token_type == REDIR_OUT
			|| utils->last_token_type == APPEND
			|| utils->last_token_type == HEREDOC))
	{
		printf("Syntax error: input cannot end");
		printf("with an operator like '|', '<', '>', '>>', or '<<'\n");
		ft_error();
	}
}
void	check_null(t_joined_lexer_list **tmp)
{
	if ((*tmp) != NULL)
		(*tmp) = (*tmp)->next;
}

t_command_block	*parser(t_joined_lexer_list *list) //lexer_list'ten komut bloğu listesine geçiyoruz
{
	t_command_block		*new_block;
	t_command_block		*command_block;
	t_command_block		*temp_block;
	t_joined_lexer_list	*temp;
	t_pipeline_utils	utils;

	utils.first_token_flg = -1; //ilk token sadece operatör mü diye kontrol ediliyor komut bloğunu en başta oluştururken
	command_block = NULL;
	temp_block = NULL;
	temp = list;
	if (temp == NULL)
		return (NULL);
	while (temp != NULL)
	{
		utils.is_cmd_pointed = 0;
		utils.first_token_flg = 0;
		new_block = init_command_block(); //yeni komut bloğu oluşturduk
		pass_cmd_blk(&command_block, &new_block, &temp_block); //sonraki komut bloğuna geçer
		first_pipe_ctrl(temp); //ilk eleman pipe olamaz kontrolü
		while (temp != NULL && temp->type != PIPE)
			loop(&temp, &temp_block, &utils);
		pipeline_available(temp, &utils);
		check_null(&temp);
	}
	return (command_block);
}
