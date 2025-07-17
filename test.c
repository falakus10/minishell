#include "minishell.h"

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
        if (temp->token > 5)
        {
            if (!temp->is_next_space && temp->next->type > 5)
            {
                array = ft_strdup(temp->token);
                while (temp->next != NULL && !temp->is_next_space && temp->next->type > 5)
			    {
				    temp = temp->next;
				    char *joined = ft_strjoin(array, temp->token);
				    free(array);
				    array = joined;
                    if (temp->next->is_next_space)
                        break;
			    }

            }
            else
            {
                current->token = temp->token;
                current->type = temp->type;
            }
        }
        else
        {
            current->token = temp->token;
            current->type = temp->type;
        }

    }
}