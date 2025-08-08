#include "minishell.h"

int	take_word(const char *input, int i, t_lexer_list *lexer_list)
{
	int	len;
	t_lexer_list *temp;

	temp = lexer_list;
	while (temp->next != NULL)
		temp = temp->next;
	len = 1;
	temp->is_next_space = 0;
	while (input[i + 1] != ' ' && input[i + 1] != '\t' && input[i + 1] != '\0')
	{
		if (is_meta(input, i + 1) != 0 || input[i + 1] == '\'' || input[i
			+ 1] == '\"')
			break ;
		else if (input[i + 1] == '|' || input[i + 1] == '<' || input[i
			+ 1] == '>')
			break ;
		i++;
		len++;
	}
	if (input[i + 1] == ' ' || input[i + 1] == '\t' )
		temp->is_next_space = 1;
	return (len);
}

int  quote_len(const char *input, int start, char delim, t_lexer_list *lexer_list)
{
	int len = 1;                  /* açılış tırnağını da saysın   */
	int i   = start + 1;
	t_lexer_list *temp;

	temp = lexer_list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->is_next_space = 0;
	while (input[i] && input[i] != delim)
	{
		++len;                    /* tırnak içindeki karakterler  */
		++i;
	}
	if (input[i] == '\0')         /* kapanış yok → hata           */
		ft_error();
	
	if (input[i + 1] == ' ' || input [i + 1] == '\t')
		temp->is_next_space = 1;
	return len;                   /* kapanış tırnağına kadarki uzunluk */
}
int is_quote(const char *input, int i, t_lexer_list *lexer_list)
{
	if (input[i] == '\'')
		return quote_len(input, i, '\'', lexer_list);   /* tek tırnak */
	else if (input[i] == '\"')
		return quote_len(input, i, '\"', lexer_list);   /* çift tırnak */
	else
		return 0;                           /* tırnak değil */
}

int	is_meta(const char *input, int i)
{
	int	meta_type;

	meta_type = 0;
	if (input[i] == '|')
		meta_type = PIPE;
	else if (input[i] == '<')
	{
		meta_type = REDIR_IN;
		if (input[i + 1] == '<')
			meta_type = HEREDOC;
	}
	else if (input[i] == '>')
	{
		meta_type = REDIR_OUT;
		if (input[i + 1] == '>')
			meta_type = APPEND;
	}
	return (meta_type);
}

void	lexer_function(t_lexer_list **lexer_list, char *input)
{
	int				i;
	char			*array;
	t_lexer_list    *current;

	*lexer_list = NULL;
	array = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		current = add_new_node(lexer_list);
		if (is_meta(input, i))
			array = meta_assign(input, &i);
		else if (is_quote(input, i, *lexer_list))
			array = quote_assign(input, &i, *lexer_list);
		else if (take_word(input, i, *lexer_list))
			array = word_assign(input, &i, *lexer_list);
		(current)->token = ft_strdup(array); //array zaten substr ile oluşturulmuştu önceki heap'ten kalan alanı free'le
		(current)->type = set_type(array);
	}
}
