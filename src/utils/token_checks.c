/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:56:09 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 15:41:08 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_first_pipe(t_joined_lexer_list *tmp, t_expander *expnd)
{
	if ((tmp != NULL) && (tmp)->type == PIPE)
	{
		write(2, "bash: syntax error near unexpected token `|'\n", 45);
		expnd->exit_value = 2;
		return (1);
	}
	return (0);
}

int	just_operator(t_joined_lexer_list *tmp, t_expander *expnd)
{
	if ((tmp->type >= 2 && tmp->type <= 5)
		&& (tmp->next == NULL || tmp->next->type == PIPE))
	{
		if (tmp->next == NULL)
		{
			write(2, "bash: syntax error near unexpected token `newline'\n", 51);
			expnd->exit_value = 2;
			return (1);
		}
		else if (tmp->next->type == PIPE)
		{	
			write(2, "bash: syntax error near unexpected token `|'\n", 45);
			expnd->exit_value = 2;
			return (1);
		}
	}
	return (0);
}

int	print_error_check(t_joined_lexer_list *tmp, t_expander *expnd)
{
	if (tmp->next == NULL)
		return (0);
	if (tmp->next->type == REDIR_IN)
		write(2, "bash: syntax error near unexpected token `<'\n", 46);
	else if (tmp->next->type == REDIR_OUT)
		write(2, "bash: syntax error near unexpected token `>'\n", 45);
	else if (tmp->next->type == APPEND)
		write(2, "bash: syntax error near unexpected token `>>'\n", 46);
	else if (tmp->next->type == HEREDOC)
		write(2, "bash: syntax error near unexpected token `<<'\n", 46);
	else if (tmp->next->type == PIPE)
		write(2, "bash: syntax error near unexpected token `|'\n", 45);
	else
		return (0);
	expnd->exit_value = 2;
	return (1);
}

int	operator_check(t_joined_lexer_list *tmp, t_expander *expnd)
{
	while (tmp->type != PIPE && tmp->next != NULL)
	{
		if ((tmp->type >= 2 && tmp->type <= 5))
		{
			if (print_error_check(tmp, expnd))
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	check_tokens(t_joined_lexer_list **temp, t_expander *expnd)
{
	t_joined_lexer_list	*tmp;

	tmp = *temp;
	if (is_first_pipe(tmp, expnd))
		return (1);
	while (tmp != NULL)
	{	
		if (just_operator(tmp, expnd) || operator_check(tmp, expnd))
			return (1);
		if ((tmp->type >= 1 && tmp->type <= 5) && tmp->next == NULL)
		{
			write(2, "bash: syntax error near unexpected token `newline'\n", 51);
			expnd->exit_value = 2;
			return (1);
		}
		if (tmp->type == PIPE && tmp->next != NULL && tmp->next->type == PIPE)
		{
			write(2, "bash: syntax error near unexpected token `|'\n", 45);
			expnd->exit_value = 2;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
