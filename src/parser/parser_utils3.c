#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

int create_path(t_command_block *tmp_blk, char *word)
{
    char *path_env;
	char **paths;
	int is_vld_pth;
	char *path;
	int i;

	path_env = getenv("PATH");
	paths = ft_split(path_env, ':');
	i = 0;
	while(paths[i] != NULL)
	{
		path = malloc(ft_strlen(paths[i]) + ft_strlen(word) + 1 + 1); //+1 / ve +1 \0 için
		ft_strcpy(path,paths[i]);
		ft_strcat(path,"/");
		ft_strcat(path,word); //ft_strcat fonksiyonu \0 ekliyor
		is_vld_pth = access(path,F_OK);
		if (!is_vld_pth)
		{
			tmp_blk->command = ft_strdup(path);
			return (1);
		}
		free(path);
		i++;
	}
	return (0);
}


