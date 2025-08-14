/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:54:16 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 12:54:46 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**get_paths_from_env(t_env *env)
{
	char	*path_env;
	char	**paths;

	path_env = take_path(env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	free(path_env);
	return (paths);
}

char	*build_path(const char *dir, const char *word)
{
	char	*tmp;
	char	*path;

	if (!ft_strchr(word, '/'))
	{
		tmp = ft_strjoin(dir, "/");
		path = ft_strjoin(tmp, word);
		free(tmp);
		return (path);
	}
	else
		return (ft_strdup(word));
}

int	try_paths(t_command_block *tmp_blk, char **paths, char *word)
{
	struct stat	st;
	char		*path;
	int			i;

	i = 0;
	while (paths && paths[i])
	{
		path = build_path(paths[i], word);
		if (stat(path, &st) == 0)
		{
			check_path_validity(tmp_blk, path);
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

int	create_path(t_command_block *tmp_blk, char *word)
{
	char	**paths;

	paths = get_paths_from_env(tmp_blk->env);
	if (!paths)
		return (0);
	return (try_paths(tmp_blk, paths, word));
}
