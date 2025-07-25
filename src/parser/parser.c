#include "minishell.h"

void remove_quotes(t_lexer_list *lexer_list)
{
	t_lexer_list *current;
	current = lexer_list;
	while (current != NULL)
	{
		if (current->type == 6 || current->type == 7)
		{
			char *trimmed = NULL;
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


/* char *token_join(t_lexer_list *lexer_list)
{
	t_lexer_list *temp;
	char *str;

	temp = lexer_list;
	str = '\0';
	while (temp != NULL)
	{
		if(!(temp->is_next_space) && temp->next != NULL)
		{
			if (temp->next->type == 6 || temp->next->type == 7 || temp->next->type == 8)
			{
				str = ft_strjoin(str, temp->token);
			}
			else
				break;
		}
		else 
			break;
		temp = temp->next;
	}
	return (str);
} */

t_joined_lexer_list **token_join(t_lexer_list *lexer_list)
{
	t_joined_lexer_list **list;
	t_lexer_list *temp;
	char *array;
	t_joined_lexer_list *current;

	list = malloc(sizeof(t_joined_lexer_list *));
	if (!list)
		return NULL;
	*list = NULL;
	temp = lexer_list;

	while (temp != NULL)
	{
		current = add_new_node2(list);

		// Quote türündeyse ve birleştirilecekse
		if (temp->type > 5)
		{
			array = ft_strdup(temp->token);

			// temp->is_next_space kontrolü, şu anki token ile sonrakinin arasında boşluk yoksa
			while (temp->next != NULL && temp->next->is_next_space == 0 && temp->next->type > 5)
			{
				temp = temp->next;
				char *joined = ft_strjoin(array, temp->token);
				free(array);
				array = joined;
				if (temp->next != NULL)
					if(temp->next->is_next_space)
						break;
			}

			current->token = array;
			current->type = WORD;

			// temp bir sonraki birleşmeye hazır hale gelsin
			temp = temp->next;
		}
		else
		{
			// Diğer durumlarda olduğu gibi kopyala
			current->token = ft_strdup(temp->token);
			current->type = temp->type;
			temp = temp->next;
		}
	}
	return list;
	
}








static char **append_to_array(char **array, int count, char *new_value)
{
	char **new_array;
	int i;

	new_array = malloc(sizeof(char *) * (count + 2)); // count + 2 olmasının sebebi : yeni eleman + NULL için
	if (!new_array)
		return NULL;
	i = 0;
	while (i < count)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[count] = ft_strdup(new_value); //temp->token'ınını direkt vermiyoruz onu strdup ile kopyalıyoruz.
	new_array[count + 1] = NULL;
	free(array);
	return (new_array);
}

t_command_block *parser(t_joined_lexer_list *list)
{
	t_command_block *command_block = NULL;
	t_command_block *temp_block = NULL;
	t_joined_lexer_list *temp = list;
	int last_token_type = -1; //komut blogunun son tokenı operator mu degil mi bakmak icin 
	//int pipe_count;
	if (temp == NULL)
		return NULL;
	while (temp != NULL)
	{
		int is_cmd_pointed = 0;
		int first_token_flg = 0;

		t_command_block *new_block = malloc(sizeof(t_command_block));
		if (!new_block)
			return NULL;

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

		if (command_block == NULL)
		{
			command_block = new_block;
			temp_block = command_block;
		}
		else
		{
			temp_block->next = new_block;
			temp_block = temp_block->next;
		}

		if (temp->type == PIPE)
		{
			printf("Syntax error: First token cannot be a pipe\n");
			ft_error();
		}

		while (temp != NULL && temp->type != PIPE)
		{
			if (first_token_flg == 0 && temp->next == NULL &&
				(temp->type == REDIR_IN || temp->type == REDIR_OUT || temp->type == APPEND || temp->type == HEREDOC))
			{
				printf("Syntax error: Operator cannot be at the end\n");
				ft_error();
			}
			first_token_flg = 1;

			if (temp->next != NULL &&
				(temp->type == REDIR_IN || temp->type == REDIR_OUT || temp->type == APPEND || temp->type == HEREDOC))
			{
				if (temp->next->type != WORD && temp->next->type != S_QUOTE && temp->next->type != D_QUOTE)
				{
					printf("Syntax error: Operator must be followed by a word\n");
					ft_error();
				}

				if (temp->type == HEREDOC) //if (temp_block->heredoc_delimiters == NULL) else durumu şeklinde yazmak daha iyi olur mu
				{
					temp_block->heredoc_delimiters = append_to_array(
						temp_block->heredoc_delimiters,
						temp_block->heredoc_count,
						temp->next->token
					);
					temp_block->heredoc_count++;
					temp = temp->next;
				}
				
				else
				{
					temp_block->files = append_to_array(
						temp_block->files,
						temp_block->operator_count,
						temp->next->token
					);
					temp_block->operator_count++;
					temp = temp->next;
				}
			}
			else if (!is_cmd_pointed && (temp->type == WORD || temp->type == S_QUOTE || temp->type == D_QUOTE))
			{
				temp_block->command = ft_strdup(temp->token);
				is_cmd_pointed = 1;
			}
			else if (is_cmd_pointed && (temp->type == WORD || temp->type == S_QUOTE || temp->type == D_QUOTE))
			{
				temp_block->args = append_to_array(
					temp_block->args,
					temp_block->argument_count,
					temp->token
				);
				temp_block->argument_count++;
			}
			last_token_type = temp->type;
			temp = temp->next;
		}

		if (temp != NULL && temp->type == PIPE && temp->next == NULL)
		{
			printf("Syntax error: input cannot end with a pipe '|'\n");
			ft_error();
		}
		if (temp == NULL && (
				last_token_type == PIPE ||
				last_token_type == REDIR_IN ||
				last_token_type == REDIR_OUT ||
				last_token_type == APPEND ||
				last_token_type == HEREDOC))
		{
			printf("Syntax error: input cannot end with an operator like '|', '<', '>', '>>', or '<<'\n");
			ft_error();
		}	

		if (temp != NULL)
			temp = temp->next;
	}
	return (command_block);
}

/*
değişken listem doğru mu ? yani mesela heredoc_delimiters , operator_count gerekli mi ? 
fd'leri kayıt etmeliyim ?  
fonksiyon bölünecek
pipe_count'u göndermedik
*/