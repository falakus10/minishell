#include "minishell.h"

int ft_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while(tmp != NULL)
	{
		write(STDOUT_FILENO, tmp->line, ft_strlen(tmp->line));
		write(STDOUT_FILENO, "\n", 1);
		tmp = tmp->next;
	}
	return (0);
}