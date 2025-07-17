#include "minishell.h"

void	remove_quotes(t_lexer_list *lexer_list)
{
	t_lexer_list	*current;
	char			*trimmed;

	current = lexer_list;
	while (current != NULL)
	{
		if (current->type == 6 || current->type == 7)
		{
			trimmed = NULL;
			if (current->type == 6)
				trimmed = ft_strtrim(current->token, "\'");
			else
				trimmed = ft_strtrim(current->token, "\"");
			if (trimmed)
			{
				free(current->token);
				current->token = trimmed;
			}
		}
		current = current->next;
	}
}

t_joined_lexer_list	*merge_words(t_lexer_list **temp,
		t_joined_lexer_list *current)
{
	char	*array;
	char	*joined;

	if (!(*temp)->is_next_space && (*temp)->next != NULL
		&& (*temp)->next->type > 5)
	{
		array = ft_strdup((*temp)->token);
		while ((*temp)->next != NULL && !(*temp)->is_next_space
			&& (*temp)->next->type > 5)
		{
			*temp = (*temp)->next;
			joined = ft_strjoin(array, (*temp)->token);
			free(array);
			array = joined;
		}
		current->token = array;
		current->type = WORD;
	}
	else
	{
		current->token = (*temp)->token;
		current->type = (*temp)->type;
	}
	return (current);
}

t_joined_lexer_list	**token_join(t_lexer_list *lexer_list)
{
	t_joined_lexer_list	**list;
	t_lexer_list		*temp;
	t_joined_lexer_list	*current;

	list = malloc(sizeof(t_joined_lexer_list *));
	if (!list)
		return (NULL);
	*list = NULL;
	temp = lexer_list;
	while (temp != NULL)
	{
		current = add_new_node2(list);
		if (temp->type > 5)
			current = merge_words(&temp, current);
		else
		{
			current->token = temp->token;
			current->type = temp->type;
		}
		temp = temp->next;
	}
	return (list);
}

static char	**append_to_array(char **array, int count, char *new_value)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * (count + 2)); // count + 2 olmasının sebebi : yeni eleman + NULL için
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[count] = ft_strdup(new_value);
		// temp->token'ınını direkt vermiyoruz onu strdup ile kopyalıyoruz.
	new_array[count + 1] = NULL;
	free(array);
	return (new_array);
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

t_command_block	*init_command_block(void)
{
	t_command_block	*new_block;

	new_block = malloc(sizeof(t_command_block));
	if (!new_block)
		return (NULL);
	new_block->next = NULL;
	new_block->args = NULL;
	new_block->fd = NULL;
	new_block->files = NULL;
	new_block->operators = NULL;
	new_block->heredoc_delimiters = NULL;
	new_block->heredoc_count = 0;
	new_block->operator_count = 0;
	new_block->argument_count = 0;
	new_block->command = NULL;
	return (new_block);
}
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
void loop(t_joined_lexer_list **tmp, t_command_block **tmp_blk, t_pipeline_utils *utils)
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
void check_null(t_joined_lexer_list **tmp)
{
	if ((*tmp) != NULL)
		(*tmp) = (*tmp)->next;
}

t_command_block	*parser(t_joined_lexer_list *list)
{
	t_command_block		*new_block;
	t_command_block		*command_block;
	t_command_block		*temp_block;
	t_joined_lexer_list	*temp;
	t_pipeline_utils	utils;

	utils.first_token_flg = -1;
	command_block = NULL;
	temp_block = NULL;
	temp = list;
	if (temp == NULL)
		return (NULL);
	while (temp != NULL)
	{
		utils.is_cmd_pointed = 0;
		utils.first_token_flg = 0;
		new_block = init_command_block();
		pass_cmd_blk(&command_block, &new_block, &temp_block);
		first_pipe_ctrl(temp);
		while (temp != NULL && temp->type != PIPE)
			loop(&temp,&temp_block,&utils);
		pipeline_available(temp, &utils);
		check_null(&temp);
	}
	return (command_block);
}



/*
değişken listem doğru mu ? yani mesela heredoc_delimiters ,
	operator_count gerekli mi ?
fd'leri kayıt etmeliyim ?
fonksiyon bölünecek
pipe_count'u göndermedik
*/