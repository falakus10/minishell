#include "minishell.h"

void init_heredoc_struct(t_mng_heredocs *mng, int count, t_joined_lexer_list **temp, t_env *env_list)
{
	mng->index = 0;
	mng->heredoc_flags = malloc(sizeof(int) * count);
	mng->heredoc_fds = malloc(sizeof(int) * count);
	mng->heredoc_nums = malloc(sizeof(int) * count);
	mng->heredoc_delims = malloc(sizeof(char *) * (count_heredoc(temp) + 1));
	mng->env = env_list;
	if (!mng->heredoc_flags || !mng->heredoc_fds || !mng->heredoc_nums || !mng->heredoc_delims)
		return; //burası return NULL'dı ama return'e çevirdim sorun olur mu ? 
	fill_int_array(mng->heredoc_flags, 0, count);
	fill_int_array(mng->heredoc_fds, -3, count); //int dolduran fonksiyonla değiştir
	fill_int_array(mng->heredoc_nums, 0, count);
}

void	fill_heredoc_flags(t_mng_heredocs *mng, t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	int i;
	t_joined_lexer_list *scan;
	int heredoc_valid;

	tmp = *temp;
	i = 0;
	heredoc_valid = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next)
		{
			if (tmp->next->type == WORD)
				heredoc_valid = 1;
			scan = tmp->next->next;

			while (scan && scan->type != PIPE)
			{
				if (scan->type == REDIR_IN)
				{
					heredoc_valid = 0;  // heredoc geçersiz oldu çünkü daha sonra < geldi
					break;
				}
				scan = scan->next;
			}
			if (heredoc_valid)
				mng->heredoc_flags[i] = 1;
		}
		if (tmp->type == PIPE)
			i++;
		tmp = tmp->next;
	}
}

void run_hrdcs(t_mng_heredocs *mng, t_joined_lexer_list **temp, t_init *init)
{
	fill_heredoc_flags(mng, temp);
	take_heredoc_delims(temp, count_heredoc(temp), &mng);
	fill_heredoc_nums(&mng, temp);
	heredoc_handle(mng, count_heredoc(temp), init);
}

