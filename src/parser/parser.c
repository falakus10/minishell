#include "minishell.h"

void	loop(t_joined_lexer_list **tmp, t_command_block **tmp_blk,
		t_pipeline_utils *utils,t_mng_heredocs *mng_heredocs)
{
	handle_token_logic(tmp, tmp_blk, utils,mng_heredocs);
}

void	check_null(t_joined_lexer_list **tmp)
{
	if ((*tmp) != NULL)
		(*tmp) = (*tmp)->next;
}

t_command_block	*parser(t_joined_lexer_list *list,t_mng_heredocs *mng_heredocs ,t_expander *expander,t_env *env) //lexer_list'ten komut bloğu listesine geçiyoruz
{
	t_command_block		*new_block;
	t_command_block		*command_block;
	t_command_block		*temp_block;
	t_joined_lexer_list	*temp;
	t_pipeline_utils	utils;

	command_block = NULL;
	temp_block = NULL;
	temp = list;
	if (temp == NULL)
		return (NULL);
	while (temp != NULL)
	{
		utils.is_cmd_pointed = 0;
		new_block = init_command_block(expander,env); //yeni komut bloğu oluşturduk
		pass_cmd_blk(&command_block, &new_block, &temp_block); //sonraki komut bloğuna geçer
		while (temp != NULL && temp->type != PIPE)
			loop(&temp, &temp_block, &utils,mng_heredocs);
		printf("fill err= %d, path err = %d\n", temp_block->file_err, temp_block->path_err);
		if(temp_block->file_err == 0 && temp_block->cmd_err == 1)
		{
			printf("bash: %s: command not found\n",(temp_block)->wrong_cmd);
			temp_block->last_fault = 1;
			temp_block->expnd->exit_value = 127;
		}
		else if(temp_block->path_err == 1 && temp_block->file_err == 0)
		{
			write(2,"bash: ", 6);
			write (2, temp_block->wrong_cmd, ft_strlen(temp_block->wrong_cmd));
			write(2, ": No such file or directory\n", 28);
			temp_block->expnd->exit_value = 127;
		}
		else if(temp_block->file_err == 0 && temp_block->command == NULL)
			temp_block->file_err = 1;
		
		mng_heredocs->index++;
		check_null(&temp);
	}
	return (command_block);
}
