/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:55:21 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 12:56:03 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_old_fd(int old_fd)
{
	if (old_fd > -1 && old_fd != -3)
		close(old_fd);
}

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

char	*take_path(t_env *env)
{
	char	*path;
	t_env	*tmp;

	tmp = env;
	path = NULL;
	while (tmp != NULL)
	{
		if (!strncmp("PATH=", tmp->line, 5))
			path = ft_strdup(tmp->line);
		tmp = tmp->next;
	}
	return (path);
}
