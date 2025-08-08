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

t_joined_lexer_list	*merge_words(t_lexer_list **temp,t_joined_lexer_list *current)
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

void	token_join(t_joined_lexer_list **new_list ,t_lexer_list *lexer_list)
{
	t_joined_lexer_list	*current;
	t_lexer_list		*temp;
	
	*new_list = NULL; //mainde var ama içeri de koydum, içeri koymayınca bozuluyordu (içerde var ama mainde yokken de çalışıyor ama mainde de kalsın dedim)
	temp = lexer_list;
	while (temp != NULL)
	{
		/* if (temp->token[0] == '\0')
		{
			temp = temp->next;
			continue;
		} */
		current = add_new_node2(new_list);
		if (temp->type > 5)
		current = merge_words(&temp, current);
		else
		{
			current->token = ft_strdup(temp->token);
			current->type = temp->type;
		}
		temp = temp->next;
	}
}

char	**append_to_array(char **array, int count, char *new_value)
{
	char	**new_array;
	int		i;
	
	if(new_value == NULL)
		return NULL;
	new_array = malloc(sizeof(char *) * (count + 2)); // count + 2 olmasının sebebi : yeni eleman + NULL için
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < count  && array) // || !array eklenmeli mi
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[count] = ft_strdup(new_value);// temp->token'ınını direkt vermiyoruz onu strdup ile kopyalıyoruz. //strdup başarısız olursa 
	new_array[count + 1] = NULL;
	free(array);
	return (new_array);
}

t_command_block	*init_command_block(t_expander *expander,t_env *environ)
{
	t_command_block	*new_block;
	
	new_block = malloc(sizeof(t_command_block));
	if (!new_block)
		return (NULL);
	new_block->next = NULL;
	new_block->args = NULL;
	new_block->fd = NULL;
	new_block->last_fault = 0;
	new_block->files = NULL;
	new_block->operators = NULL;
	new_block->heredoc_fd = malloc(sizeof(int));
	new_block->heredoc_count = 0;
	new_block->operator_count = 0;
	new_block->argument_count = 0;
	new_block->cmd_count = 0;
	new_block->input_fd = -3;//öylesine -3 ile başlattım önemli mi ?
	new_block->output_fd = -3;
	new_block->err_flg = -2; //sanırım kullanılmıyor //öylesine -2 ile başlattım önemli mi ?
	new_block->err_sign = 0; // sanırım kullanılmıyor
	new_block->file_err = 0;
	new_block->cmd_err = 0;
	new_block->command = NULL;
	new_block->wrong_cmd = NULL;
	new_block->expnd = expander;
	new_block->path_err = 0;
	new_block->wrong_path = 0;
	new_block->env = environ;
	return (new_block);
}
