#include "minishell.h"

//bu freelere gerek var mı?
char **free_heredoc_delimiters(char **delims, int last_index) //gerek var mı ?
{
	while (--last_index >= 0)
		free(delims[last_index]);
	free(delims);
	return (NULL);
}

void take_heredoc_delims(t_joined_lexer_list **temp, int heredoc_count,t_mng_heredocs **mng_heredocs)
{
	t_joined_lexer_list *tmp;
	int i = 0;
	tmp = *temp;
	while (tmp && i < heredoc_count)
	{
		if (tmp->type == HEREDOC && tmp->next) //if (tmp->type == HEREDOC && tmp->next && tmp->next->type)
		{
			if (tmp->next->type == WORD || tmp->next->type == S_QUOTE || tmp->next->type == D_QUOTE)
			{
				(*mng_heredocs)->heredoc_delims[i] = ft_strdup(tmp->next->token);
				if (!(*mng_heredocs)->heredoc_delims[i])
					free_heredoc_delimiters((*mng_heredocs)->heredoc_delims, i); //gerek var mı 
				i++;
			}
		}
		tmp = tmp->next;
	}
	(*mng_heredocs)->heredoc_delims[i] = NULL;
}

void fill_heredoc_nums(t_mng_heredocs **mng_heredocs, t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp = *temp;
	int i = 0;

	while (tmp != NULL)
	{
		if (tmp->type == HEREDOC && tmp->next)
			if(tmp->next->type == WORD || tmp->next->type == D_QUOTE || tmp->next->type == S_QUOTE)
				(*mng_heredocs)->heredoc_nums[i]++;
		if (tmp->type == PIPE)
			i++;

		tmp = tmp->next;
	}
}

int count_heredoc(t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	int heredoc_count;

	tmp = *temp;
	heredoc_count = 0;
	while(tmp != NULL)
	{
		if(tmp->type == HEREDOC && tmp->next)
			if (tmp->next->type == WORD || tmp->next->type == D_QUOTE || tmp->next->type == S_QUOTE)
				heredoc_count++;
		tmp = tmp->next;
	}
	return (heredoc_count);
}

int count_cmd_blk(t_joined_lexer_list **temp)
{
	int pipe_count;
	t_joined_lexer_list *tmp;

	pipe_count = 0;
	tmp = (*temp);
	while((tmp) != NULL) //tmp->next != NULL da olur ama böyle kalabilir 
	{
		if(tmp->type == PIPE)
			pipe_count++;
		tmp=tmp->next;
	}
	return (pipe_count + 1);
}
