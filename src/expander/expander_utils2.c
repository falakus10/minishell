/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:23:11 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:23:12 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_env_from_token(char *token, int dollar_index, int key_len)
{
	char	*before;
	char	*after;
	char	*new_token;

	before = ft_substr(token, 0, dollar_index);
	after = ft_strdup(token + dollar_index + key_len + 1);
	new_token = ft_strjoin(before, after);
	free(before);
	free(after);
	free(token);
	return (new_token);
}

int	special_character(char *token, t_expander *expander)
{
	if (token[expander->i] == '$')
	{
		expander->i++;
		return (1);
	}
	else if (special_ch_check(token[expander->i]))
	{
		expander->i++;
		while (special_ch_check(token[expander->i]))
			expander->i++;
		while (token[expander->i] != '\0' && (is_valid_ch(token, expander->i)))
			expander->i++;
		return (1);
	}
	return (0);
}

int	question_mark(t_lexer_list *temp, int i, t_expander *expander)
{
	char	*line;

	if (temp->token[i] == '?')
	{
		expander->start = i;
		while (temp->token[i] != '\0' && is_valid_ch(temp->token, i))
			i++;
		expander->env_key = "?";
		expander->env_val = ft_itoa(expander->exit_value);
		line = ft_strdup(temp->token);
		free(temp->token);
		temp->token = ft_strjoin_free(line, expander);
		free(line);
		expander->i = 0;
		free(expander->env_val);
		return (1);
	}
	return (0);
}
