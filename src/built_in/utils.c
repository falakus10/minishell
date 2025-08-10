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

void	sort_and_print(t_env **arr, int count)
{
	int		i;
	int		j;
	char	*line;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if(arr[j]->line == NULL)
				printf("NULL\n");
			if (ft_strcmp(arr[i]->line, arr[j]->line) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		line = format_export_line(arr[i]);
		write(1, "declare -x ", 11);
		write(1, line, ft_strlen(line));
		write(1, "\n", 1);
		free(line);
		i++;
	}
}
