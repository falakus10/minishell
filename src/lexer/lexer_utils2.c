/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:10:46 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 13:13:59 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*word_assign(const char *input, int *inx, t_lexer_list *lexer_list)
{
	char	*token;
	int		len;

	token = NULL;
	len = take_word(input, *inx, lexer_list);
	token = ft_substr(input, *inx, len);
	(*inx) += len;
	return (token);
}
