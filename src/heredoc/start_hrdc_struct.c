#include "minishell.h"

t_mng_heredocs *init_heredoc_struct(int count, t_joined_lexer_list **temp)
{
	t_mng_heredocs *mng;

	mng = malloc(sizeof(t_mng_heredocs));
	if (!mng)
		return (NULL);
	mng->index = 0;
	mng->heredoc_flags = malloc(sizeof(int) * count);
	mng->heredoc_fds = malloc(sizeof(int) * count);
	mng->heredoc_nums = malloc(sizeof(int) * count);
	mng->heredoc_delims = malloc(sizeof(char *) * (count_heredoc(temp) + 1));
	if (!mng->heredoc_flags || !mng->heredoc_fds || !mng->heredoc_nums || !mng->heredoc_delims)
		return (NULL);
	ft_memset(mng->heredoc_flags, 0, sizeof(int) * count);
	ft_memset(mng->heredoc_fds, -1, sizeof(int) * count);
	ft_memset(mng->heredoc_nums, 0, sizeof(int) * count);
	return (mng);
}

void	fill_heredoc_flags(t_mng_heredocs *mng, t_joined_lexer_list **temp)
{
	t_joined_lexer_list *tmp;
	int i;
	t_joined_lexer_list *scan;
	int heredoc_valid;

	tmp = *temp;
	i = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next)
		{
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

t_mng_heredocs *run_hrdcs(t_joined_lexer_list **temp, int cmd_blk_count)
{
	t_mng_heredocs *mng;

	mng = init_heredoc_struct(cmd_blk_count, temp);
	fill_heredoc_flags(mng, temp);
	take_heredoc_delims(temp, count_heredoc(temp), &mng);
	fill_heredoc_nums(&mng, temp);
	heredoc_handle(mng, count_heredoc(temp));
	return (mng);
}

