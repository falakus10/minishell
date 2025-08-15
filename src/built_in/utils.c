/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:22:57 by falakus           #+#    #+#             */
/*   Updated: 2025/08/15 11:48:04 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*format_export_line(t_env *node)
{
	int		index;
	char	*equal_pos;
	char	*q_value;
	char	*result;
	size_t	result_size;

	index = -1;
	equal_pos = ft_strchr(node->line, '=');
	if (equal_pos)
		index = equal_pos - node->line;
	if (index == -1)
		return (ft_strdup(node->line));
	q_value = add_quotes(node->value);
	result_size = index + 1 + ft_strlen(q_value) + 1;
	result = malloc(result_size);
	if (!result)
		ft_error();
	ft_strncpy(result, node->line, index + 1);
	result[index + 1] = '\0';
	ft_strlcat(result, q_value, result_size);
	free(q_value);
	return (result);
}

void	sort_and_print(t_env **arr, int count, int i)
{
	int		j;
	t_env	*temp;
	char	*formatted;

	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strcmp(arr[i]->line, arr[j]->line) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
	i = -1;
	while (++i < count)
	{
		formatted = format_export_line(arr[i]);
		write_message("declare -x ", formatted, NULL, 1);
		free(formatted);
	}
}
