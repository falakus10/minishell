#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (0);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd)) && ft_strlen(cmd) == 2)
		return (1);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)) && ft_strlen(cmd) == 6)
		return (2);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd)) && ft_strlen(cmd) == 5)
		return (3);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd)) && ft_strlen(cmd) == 4)
		return (4);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd)) && ft_strlen(cmd) == 4)
		return (5);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd)) && ft_strlen(cmd) == 3)
		return (6);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd)) && ft_strlen(cmd) == 3)
		return (7);
	return (0);
}

char *take_path(t_env *env)
{
	char *path;

	path = NULL;
	while(env != NULL)
	{
		if(!strncmp("PATH=",env->line,5))
			path = ft_strdup(env->line);
		env = env->next;
	}
	return (path);
}

void	check_path_validity(t_command_block *tmp_blk, char *path)
{
	struct stat	st;

	stat(path, &st);
	if (S_ISDIR(st.st_mode))
	{
		write(2, "bash: ", 6);
		write(2, path, ft_strlen(path));
		write(2, ": Is a directory\n", 17);
		tmp_blk->wrong_path = 1;
		tmp_blk->expnd->exit_value = 126;
	}
	if (!(st.st_mode & S_IXUSR))
	{
		write(2, "bash: ", 6);
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
		tmp_blk->wrong_path = 1;
		tmp_blk->expnd->exit_value = 126;
	}
}

int create_path(t_command_block *tmp_blk, char *word, int i)
{
	char		*path_env;
	char		**paths;
	char		*tmp;
	char		*path;
	struct stat	st;

	path_env = take_path(tmp_blk->env);
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (0);
	free(path_env);
	while (paths[i])
	{
		if (!ft_strchr(word, '/'))
		{
			tmp = ft_strjoin(paths[i], "/");
			path = ft_strjoin(tmp, word);
			free(tmp);
		}
		else
			path = ft_strdup(word);
		if (stat(path, &st) == 0)
		{
			check_path_validity(tmp_blk ,path);
			tmp_blk->command = ft_strdup(path);
			free(path);
			free_arr(paths);
			return (1);
		}
		free(path);
		i++;
	}
	free_arr(paths);
	return (0);
}
