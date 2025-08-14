/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:09:16 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 13:13:40 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer_list	*add_new_node(t_lexer_list **lexer_list)
{
	t_lexer_list	*node;
	t_lexer_list	*temp;

	node = malloc(sizeof(t_lexer_list));
	node->next = NULL;
	node->token = NULL;
	if (*lexer_list == NULL)
		*lexer_list = node;
	else
	{
		temp = *lexer_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
	return (node);
}

t_joined_lexer_list	*add_new_node2(t_joined_lexer_list **lexer_list)
{
	t_joined_lexer_list	*node;
	t_joined_lexer_list	*temp;

	node = malloc(sizeof(t_joined_lexer_list));
	node->next = NULL;
	if (*lexer_list == NULL)
		*lexer_list = node;
	else
	{
		temp = *lexer_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
	return (node);
}

int	set_type(char *array)
{
	int	i;
	int	flag;

	i = 0;
	if (array[i] == '\'')
		flag = S_QUOTE;
	else if (array[i] == '\"')
		flag = D_QUOTE;
	else if (array[i] == '|')
		flag = 1;
	else if (array[i] == '<')
	{
		flag = REDIR_IN;
		if (array[i + 1] == '<')
			flag = HEREDOC;
	}
	else if (array[i] == '>')
	{
		flag = REDIR_OUT;
		if (array[i + 1] == '>')
			flag = APPEND;
	}
	else
		flag = WORD;
	return (flag);
}

char	*meta_assign(const char *input, int *inx)
{
	char	*token;
	int		type;

	token = NULL;
	type = is_meta(input, *inx);
	if (type == HEREDOC || type == APPEND)
	{
		token = ft_substr(input, *inx, 2);
		*inx += 2;
	}
	else
	{
		token = ft_substr(input, *inx, 1);
		*inx += 1;
	}
	return (token);
}

char	*quote_assign(const char *input, int *inx, t_lexer_list *lexer_list)
{
	char	*token;
	int		len;

	token = NULL;
	len = (is_quote(input, *inx, lexer_list)) + 1;
	token = ft_substr(input, *inx, len);
	*(inx) += len;
	return (token);
}
