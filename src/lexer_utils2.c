#include "minishell.h"


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

char	*quote_assign(const char *input, int *inx)
{
	char	*token;
	int		len;

	token = NULL;
	len = (is_quote(input, *inx)) + 1; //son tırnak dahil olsun diye +1 dedik. Son tırnak dahil toplam "..." uzunluğu
	token = ft_substr(input, *inx, len);
	*(inx) += len;
	return (token);
}

char *word_assign(const char *input, int *inx)
{
	char	*token;
	int		len;

	token = NULL;
	len = take_word(input, *inx);  //take_word fonksiyonunu iki kez çağırmayalım diye sonucunu len değişkenine atadım. 
	token = ft_substr(input, *inx, len);
	(*inx) +=len;

	return (token);
}