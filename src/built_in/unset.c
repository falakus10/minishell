/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:22:52 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:22:53 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_node(t_env **env, char *key)
{
	t_env	*curr;
	t_env	*prev;
	int		len;

	curr = (*env);
	prev = NULL;
	len = ft_strlen(key);
	while (curr)
	{
		if (ft_strncmp(curr->line, key, len) == 0
			&& (curr->line[len] == '=' || curr->line[len] == '\0'))
		{
			if (!prev)
				*env = curr->next;
			else
				prev->next = curr->next;
			free(curr->line);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(t_command_block *cmd, t_env **env)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		delete_node(env, cmd->args[i]);
		i++;
	}
	return (0);
}
