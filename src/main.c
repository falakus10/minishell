#include "minishell.h"

int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	t_lexer_list	**list;

	list = input_loop();
	while ((*list)->next != NULL)
	{
		printf("token = %s type = %d\n ", (*list)->token, (*list)->type);
		*list = (*list)->next;
	}
	return 0;
}