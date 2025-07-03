#include "minishell.h"

int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	t_lexer_list	*temp;
	t_lexer_list	**list;
	list = input_loop();
	temp = *list;
	
	while (temp != NULL)
	{
		printf("token = %s type = %d\n ", temp->token, temp->type);
		temp = temp->next;
	}
	return 0;
}