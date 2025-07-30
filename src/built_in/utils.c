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
		return (ft_strdup(node->line)); // '=' yoksa direkt kopyala
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
