/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:24:39 by austunso          #+#    #+#             */
/*   Updated: 2025/08/14 18:32:33 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_handle(t_mng_heredocs *mng, int i, int j, t_init *init)
{
	int		fd[2];
	pid_t	pid;
	int		k;
	int		count;
	int		h_c;

	k = -1;
	h_c = 0;
	count = count_cmd_blk(init->jnd_lxr_lst);
	while (++i < count)
	{
		j = -1;
		while (++j < mng->heredoc_nums[i])
		{
			create_pipe_or_exit(fd);
			fork_or_exit(&pid);
			if (pid == 0)
				handle_child(mng, mng->heredoc_delims[h_c], fd[1], init);
			else if (handle_parent_process(mng, fd, i, &k))
				return (1);
			h_c++;
		}
	}
	return (0);
}
