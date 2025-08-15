/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:23:16 by falakus           #+#    #+#             */
/*   Updated: 2025/08/15 13:04:58 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	not_env_val(t_lexer_list *temp, t_expander *expander)
{
	char	*line;

	line = ft_strdup(temp->token);
	free(temp->token);
	temp->token = ft_strjoin_free(line, expander);
	free(line);
	free(expander->env_key);
	free(expander->env_val);
	expander->i = 0;
}

int	change_to_env(t_lexer_list *temp, int i, t_expander *expander,
					t_env *env_list)
{
	expander->start = i;
	while (temp->token[i] != '\0' && (is_valid_ch(temp->token, i)))
		i++;
	expander->env_key = ft_substr(temp->token, expander->start,
			i - expander->start);
	expander->env_val = env_value(env_list, expander->env_key);
	if (expander->env_val != NULL)
	{
		not_env_val(temp, expander);
		return (1);
	}
	else
	{
		temp->token = remove_env_from_token(temp->token, expander->start - 1,
				i - expander->start);
		free(expander->env_key);
		free(expander->env_val);
		expander->i = expander->start - 2;
	}
	return (0);
}

int	quote(char *token, t_expander *expander)
{
	if (token[expander->i] == '\'')
	{
		expander->i++;
		while (token[expander->i] != '\'' && token[expander->i] != '\0')
			expander->i++;
		return (1);
	}
	return (0);
}

int	exp_check(t_lexer_list **temp, t_expander *expander, t_env *env_list)
{
	if (((*temp)->type >= 2 && (*temp)->type <= 4) && (*temp)->next != NULL)
	{
		if ((*temp)->next->token[0] == '$')
		{
			(*temp) = (*temp)->next;
			if (!env_value(env_list, (*temp)->token + 1))
				expander->i = ft_strlen((*temp)->token);
			else
				return (1);
		}
	}
	else if ((*temp)->token[expander->i] == '$')
	{
		expander->dollar_index = expander->i++;
		if ((*temp)->token[expander->i] == '\0' || (*temp)->token[expander->i]
			== '\"' || (*temp)->token[expander->i] == ' ')
			return (1);
		if (special_character((*temp)->token, expander))
			return (1);
		if (question_mark((*temp), expander->i, expander))
			return (1);
		if (change_to_env((*temp), expander->i, expander, env_list))
			return (1);
	}
	return (0);
}

void	expander(t_lexer_list *temp, t_env *env_list, t_expander *expander)
{
	if (temp == NULL || temp->token == NULL)
		return ;
	expander->token_len = ft_strlen(temp->token);
	while (temp != NULL)
	{
		expander->i = 0;
		while (temp->token[expander->i] != '\0')
		{
			if (quote(temp->token, expander))
				continue ;
			if (temp->type == HEREDOC && temp->next != NULL)
			{
				temp = temp->next;
				expander->i = ft_strlen(temp->token) - 1;
			}
			else if (exp_check(&temp, expander, env_list))
				continue ;
			expander->i++;
		}
		temp = temp->next;
	}
}
