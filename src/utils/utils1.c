/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:41:56 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 15:42:59 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_env	*add_new_node3(t_env **env_list)
{
	t_env	*node;
	t_env	*temp;

	node = malloc(sizeof(t_env));
	if (!node)
		ft_error();
	node->next = NULL;
	node->value = NULL;
	node->flag = 0;
	if (*env_list == NULL)
		*env_list = node;
	else
	{
		temp = *env_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
	return (node);
}

t_env	**take_env(t_env **env_list, char **env)
{
	int		i;
	int		j;
	t_env	*current;

	*env_list = NULL;
	i = -1;
	while (env[++i] != NULL)
	{
		j = -1;
		current = add_new_node3(env_list);
		current->line = ft_strdup(env[i]);
		while (env[i][++j] != '\0')
		{
			if (env[i][j] == '=')
			{
				current->flag = 1;
				current->value = ft_strdup(&env[i][j + 1]);
				break ;
			}
		}
	}
	return (env_list);
}
