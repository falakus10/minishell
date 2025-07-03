#include "minishell.h"

int	is_meta(char *input, int i)
{
	int	meta_type;

	meta_type = 0;
	if (input[i] == PIPE)
		meta_type = PIPE;
	else if (input[i] == REDIR_IN)
	{
		meta_type = REDIR_IN;
		if (input[i + 1] == REDIR_IN)
			meta_type = HEREDOC;
	}
	else if (input[i] == REDIR_OUT)
	{
		meta_type = REDIR_OUT;
		if (input[i + 1] == REDIR_OUT)
			meta_type = APPEND;
	}
	return (meta_type);
}

int	take_word(char *input, int i)
{
	int len;

	len = 1;
	while (input[i + 1] != ' ' && input[i + 1] != '\t' && input[i + 1] != '\0')
	{
		if (is_meta(input, i + 1) != 0 || input[i+1] == '\'' || input[i+1] == '\"')
			break;
		i++;
		len++;
	}
	return (len);
}

int	is_quote(char *input, int i)
{
	int	len;

	len = 1; // ilk tırnağı karakter olarak saysın diye 1 den başlattık
	if (input[i++] == '\'')
	{
		while (input[i++] != '\'')
		{
			if (input[i] == '\0')
				return (0);
			len++; // bu len aslında sadece tırnak içini sayıyor
		}
	}
	else if (input[i++] == '\"')
	{
		while (input[i++] != '\"')
		{
			if (input[i] == '\0')
				return (0);
			len++;
		}
	}
	return (len + 1); // son tırnağı karakter olarak saysın diye 1 den başlattık
}

t_lexer_list	**lexer_function(char *temporary_input)
{
	int		i;
	char	*input;
	char	*array;
	t_lexer_list **lexer_list;

	lexer_list = malloc(sizeof(t_lexer_list *));
	*lexer_list = NULL;
	i = 0;
	input = ft_strtrim(temporary_input, " "); // input loop içinde yapılabilir
	while (input[i] != '\0')
	{
		if (is_meta(input, i))
		{
			if (is_meta(input, i) == HEREDOC || is_meta(input, i) == APPEND)
			{
				array = ft_substr(input, i, 2);
				i += 2;
			}
			else
			{
				array = ft_substr(input, i, 1);
				i++;
			}
		}
		else if (input[i] == ' ' || input[i] == '\t')// boşluklar döngüyle atlanabilir
			i++;
		else if (is_quote(input, i))
		{	
			if (is_quote(input, i) == 0)
				//ft_error(); //sonra yazılacak 
			array = ft_substr(input, i, is_quote(input, i));
			i += is_quote(input, i);
		}
		else if (take_word(input, i))
		{
			array = ft_substr(input, i, take_word(input,i));
			i += take_word(input, i);
		}
		add_new_node(lexer_list,array);
	}
	return(lexer_list);
}

t_lexer_list	**input_loop(void)
{
	char *input;
	t_lexer_list **list;

	while (1)
	{
		input = readline("minishell>");
		list = lexer_function(input);
		break;
	}
	return (list);
}