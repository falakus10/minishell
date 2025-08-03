#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (0);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd)))
		return (1);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)))
		return (2);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd)))
		return (3);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd)))
		return (4);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd)))
		return (5);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd)))
		return (6);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd)))
		return (7);
	return (0);
}

void	check_path_validity(char *path)
{
	struct stat	st;

	stat(path, &st);
	if (S_ISDIR(st.st_mode))
	{
		printf("%s: Is a directory\n", path);
		// ft_error();
	}
	if (!(st.st_mode & S_IXUSR))
	{
		printf("%s: Permission denied\n", path);
		// ft_error();
	}
}

int create_path(t_command_block *tmp_blk, char *word, int i) //buranın leaklerine bak
{
	char		*path_env;
	char		**paths;
	char		*tmp;
	char		*path;
	struct stat	st;

	path_env = getenv("PATH"); //düzeltilmeli mi ?
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (0);
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
			check_path_validity(path);
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



