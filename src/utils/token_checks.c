#include "minishell.h"

void is_first_pipe(t_joined_lexer_list *tmp) //ilk eleman pipe olamaz
{
	if((tmp != NULL) && (tmp)->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		ft_error();
	}
}


void just_operator(t_joined_lexer_list *tmp) //sadece operatör (< ) olamaz veya < | gibi olamaz  
{
	if((tmp->type >= 2 && tmp->type <=5) && (tmp->next == NULL || tmp->next->type == PIPE)) //type>=1 olabilir?
	{
		if(tmp->next == NULL)
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			ft_error();
		}
		else if(tmp->next->type == PIPE)
		{	
			printf("bash: syntax error near unexpected token `|'\n");
			ft_error();
		}
	}
}

void	print_error_check(t_joined_lexer_list *tmp)
{
	if (tmp->next == NULL)
		return ;
	if (tmp->next->type == REDIR_IN)
		printf("bash: syntax error near unexpected token `<'\n");
	else if (tmp->next->type == REDIR_OUT)
		printf("bash: syntax error near unexpected token `>'\n");
	else if (tmp->next->type == APPEND)
		printf("bash: syntax error near unexpected token `>>'\n");
	else if (tmp->next->type == HEREDOC)
		printf("bash: syntax error near unexpected token `<<'\n");
	else if (tmp->next->type == PIPE)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		return ;
	ft_error(); // sadece hata varsa çağrılır
}

void check_tokens(t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	
	tmp = *temp;
	is_first_pipe(tmp);
	while(tmp != NULL)
	{	
		just_operator(tmp);
		while (tmp->type != PIPE && tmp->next != NULL)
		{
			if((tmp->type >= 2 && tmp->type <=5))
				print_error_check(tmp);
		tmp = tmp->next;
		}
		if((tmp->type >= 1 && tmp->type <=5) && tmp->next == NULL) //Pipe dahil, girdi pipe ve diğer operatörlerle sonlanamaz
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			ft_error();
		}
	tmp = tmp->next;
	}
}