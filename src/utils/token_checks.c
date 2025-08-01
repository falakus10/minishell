#include "minishell.h"

int is_first_pipe(t_joined_lexer_list *tmp) //ilk eleman pipe olamaz
{
	if((tmp != NULL) && (tmp)->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (1); //hata varsa return (1)
	}
	return (0); //hata yoksa return (0)
}


int just_operator(t_joined_lexer_list *tmp) //sadece operatör (< ) olamaz veya < | gibi olamaz  
{
	if((tmp->type >= 2 && tmp->type <=5) && (tmp->next == NULL || tmp->next->type == PIPE)) //type>=1 olabilir?
	{
		if(tmp->next == NULL)
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
		else if(tmp->next->type == PIPE)
		{	
			printf("bash: syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

int	print_error_check(t_joined_lexer_list *tmp)
{
	if (tmp->next == NULL) //bundan önce bu kontrol var zaten o yüzden aslında gereksiz
		return (0);
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
		return (0); //hata yoksa
	return (1); //hata varsa
}


int check_tokens(t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	
	tmp = *temp;
	if(is_first_pipe(tmp))//girdinin ilk elemanı pipe olamaz
		return (1);
	while(tmp != NULL)
	{	
		if(just_operator(tmp))
			return (1);
		while (tmp->type != PIPE && tmp->next != NULL)
		{
			if((tmp->type >= 2 && tmp->type <=5)) //ard arda operatör gelemez
			{
				if(print_error_check(tmp))
					return (1);
			}
		tmp = tmp->next;
		}
		if((tmp->type >= 1 && tmp->type <=5) && tmp->next == NULL) //Pipe dahil, girdi pipe ve diğer operatörlerle sonlanamaz
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
	tmp = tmp->next;
	}
	return (0);
}
