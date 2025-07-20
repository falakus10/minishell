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

char	**append_to_array(char **array, int count, char *new_value)
{
	char	**new_array;
	int		i;
	
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

int *append_to_array2(int *array, int count, int new_value) //fd'leri tutan dizi
{
	int *new_array;
	int i;

	new_array = malloc(sizeof(int) *(count + 1));
	if(!new_array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = new_value;
	free(array);
	return (new_array);
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
	new_block->fd_count = 0;
	new_block->input_fd = 0;
	new_block->output_fd = 0;
	new_block->command = NULL;
	return (new_block);
}
