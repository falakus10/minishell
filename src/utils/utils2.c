/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:43:19 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 15:44:49 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	fill_int_array(int *arr, int value, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		arr[i] = value;
		i++;
	}
}

int	is_space_or_tab(char c)
{
	return (c == ' ' || c == '\t');
}

char	*trim_whitespace(const char *input)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*trimmed;

	start = 0;
	while (input[start] && is_space_or_tab(input[start]))
		start++;
	end = 0;
	while (input[end])
		end++;
	if (end == 0)
		return (strdup(""));
	end--;
	while (end > start && is_space_or_tab(input[end]))
		end--;
	trimmed = (char *)malloc(end - start + 2);
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start <= end)
		trimmed[i++] = input[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

void	write_message(char *msg1, char *msg2, char *msg3, int fd)
{
	if (msg1)
		write(fd, msg1, ft_strlen(msg1));
	if (msg2)
		write(fd, msg2, ft_strlen(msg2));
	if (msg3)
		write(fd, msg3, ft_strlen(msg3));
	write(fd, "\n", 1);
}
