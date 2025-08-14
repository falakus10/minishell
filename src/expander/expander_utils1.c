/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:23:06 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:32:06 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*ft_strjoin_free(char *token, t_expander *exp)
{
	int		j;
	char	*new_token;

	j = 0;
	exp->key_len = ft_strlen(exp->env_key);
	exp->val_len = ft_strlen(exp->env_val);
	if (!token || !exp->env_val)
		return (NULL);
	exp->new_len = ft_strlen(token) + (exp->val_len - exp->key_len);
	new_token = malloc(exp->new_len + 1);
	if (!new_token)
		return (NULL);
	exp->index = find_index(token, exp->env_key, exp);
	if (exp->index == -1)
		return (NULL);
	if (token[exp->index - 1] == '$')
	{
		ft_memcpy(new_token, token, (exp->index - 1));
		ft_memcpy(new_token + (exp->index - 1), exp->env_val, exp->val_len);
		j = exp->index + exp->key_len;
		exp->index += exp->val_len - 1;
		ft_memcpy(new_token + exp->index, token + j, ft_strlen(token) - j);
	}
	exp->i = 0;
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
