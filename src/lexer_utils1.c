#include "minishell.h"

void	add_new_node(t_lexer_list **lexer_list, char *array)
{
	t_lexer_list	*node;
	t_lexer_list	*temp;
	node = malloc(sizeof(t_lexer_list));
	//if (node == NULL)
		//ft_error();
	//printf("%s\n", array);
	node->token = ft_strdup(array);
	node->type = set_type(array);
	node->next = NULL;
	if (*lexer_list == NULL)
	{
		*lexer_list = node;
	}
	else
	{
		temp = *lexer_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
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