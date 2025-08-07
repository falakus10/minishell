#include "minishell.h"

char	**env_list_to_envp(t_env **env_list)
{
	char	**envp;
	int		count;
	int		i;
	t_env	*tmp;

	i = 0;
	count = 0;
	tmp = *env_list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		ft_error();
	tmp = (*env_list);
	while (tmp)
	{
		if (!*tmp->line)
		{
			tmp = tmp->next;
			continue;
		}		
		envp[i++] = ft_strdup(tmp->line);
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}
