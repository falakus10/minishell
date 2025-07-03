#include "minishell.h"

void	add_new_node(t_lexer_list **lexer_list, char *array)
{
	t_lexer_list	*node;
	t_lexer_list	*temp;

	node = malloc(sizeof(t_lexer_list)); //null kontrolü
	temp = *lexer_list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next =node;
	node->next = NULL;
	node->token = array;
	node->type = set_type(array);
}

int	set_type(char *array)
{
	int	i;
	int	flag;

	i = 0;
	if (array[i] == S_QUOTE)
		flag = S_QUOTE;
	else if (array[i] == D_QUOTE)
		flag == D_QUOTE;
	else if (array[i] == PIPE)
		flag == PIPE;
	else if (array[i] == REDIR_IN)
	{
		flag = REDIR_IN;
		if (array[i + 1] == REDIR_IN)
			flag = HEREDOC;
	}
	else if (array[i] == REDIR_OUT)
	{
		flag = REDIR_OUT;
		if (array[i + 1] == REDIR_OUT)
			flag = APPEND;
	}
	else
		flag = WORD;
	return (flag);
}
