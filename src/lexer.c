#include "minishell.h"

int	take_word(const char *input, int i)
{
	int	len;

	len = 1;
	while (input[i + 1] != ' ' && input[i + 1] != '\t' && input[i + 1] != '\0')
	{
		if (input[i] == '\\' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		{
			len++;
		}
		else if (is_meta(input, i + 1) != 0 || input[i + 1] == '\'' || input[i
			+ 1] == '\"')
			break ;
		else if (input[i + 1] == '|' || input[i + 1] == '<' || input[i
			+ 1] == '>')
			break ;
		i++;
		len++;
	}
	return (len);
}

int  quote_len(const char *input, int start, char delim)
{
	int len = 1;                  /* açılış tırnağını da saysın   */
	int i   = start + 1;

	while (input[i] && input[i] != delim)
	{
		if (input[i] == '\\' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		{
			i++;
			len++;
		}
		++len;                    /* tırnak içindeki karakterler  */
		++i;
	}

	if (input[i] == '\0')         /* kapanış yok → hata           */
		ft_error();

	return len;                   /* kapanış tırnağına kadarki uzunluk */
}

int is_quote(const char *input, int i)
{
	if (input[i] == '\'')
		return quote_len(input, i, '\'');   /* tek tırnak */
	else if (input[i] == '\"')
		return quote_len(input, i, '\"');   /* çift tırnak */
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

t_lexer_list	**lexer_function(char *input)
{
	int				i;
	char			*array;
	t_lexer_list	**lexer_list;

	lexer_list = malloc(sizeof(t_lexer_list *));
	*lexer_list = NULL;
	array = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (is_meta(input, i))
			array = meta_assign(input, &i);
		else if (is_quote(input, i))
			array = quote_assign(input, &i);
		else if (take_word(input, i))
			array = word_assign(input, &i);
		add_new_node(lexer_list, array);
	}
	return (lexer_list);
}