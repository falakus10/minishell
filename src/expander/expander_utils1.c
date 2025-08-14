#include "minishell.h"

int	find_index(const char *haystack, const char *needle, t_expander *expander)
{
	char	*ptr;

	ptr = ft_strnstr(haystack + expander->dollar_index,
			needle, ft_strlen(haystack));
	if (ptr == NULL)
		return (-1);
	return (ptr - haystack);
}

char	*ft_strjoin_free(char *token, t_expander *expander)
{
	int		j;
	char	*new_token;

	j = 0;
	expander->key_len = ft_strlen(expander->env_key);
	expander->val_len = ft_strlen(expander->env_val);
	if (!token || !expander->env_val)
		return (NULL);
	expander->new_len = ft_strlen(token) + (expander->val_len - expander->key_len);
	new_token = malloc(expander->new_len + 1);
	if (!new_token)
		return (NULL);
	expander->index = find_index(token, expander->env_key, expander);
	if (expander->index == -1)
		return (NULL);
	if (token[expander->index - 1] == '$')
	{
		ft_memcpy(new_token, token, (expander->index - 1));
		ft_memcpy(new_token + (expander->index - 1), expander->env_val, expander->val_len);
		j = expander->index + expander->key_len;
		expander->index += expander->val_len - 1;
		ft_memcpy(new_token + expander->index, token + j, ft_strlen(token) - j);
	}
	expander->i = 0;
	return (new_token);
}

char	*env_value(t_env *env_list, const char *key)
{
	size_t	key_len;
	char	*env_value;
	t_env	*temp;

	temp = env_list;
	if (temp == NULL || key == NULL)
		return (0);
	key_len = ft_strlen(key);
	while (temp != NULL)
	{
		if (ft_strncmp(temp->line, key, key_len) == 0
			&& temp->line[key_len] == '=')
		{
			env_value = ft_substr(temp->line, key_len + 1,
					ft_strlen(temp->line) - (key_len + 1));
			return (env_value);
		}
		temp = temp->next;
	}
	return (NULL);
}

int	is_valid_ch(char *token, int i)
{
	if (ft_isalnum(token[i]) || token[i] == '_')
		return (1);
	return (0);
}

int	special_ch_check(char c)
{
	if (c >= '1' && c <= '9')
		return (1);
	else if (c == '#')
		return (1);
	else if (c == '*')
		return (1);
	else if (c == '@')
		return (1);
	else if (c == '-')
		return (1);
	else if (c == '$')
		return (1);
	else if (c == '!')
		return (1);
	else
		return (0);
}
