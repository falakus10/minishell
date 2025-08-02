#include "minishell.h"

void	handle_child_process(char *delim, int write_fd)
{
	char *line;

	close(write_fd - 1); // read ucunu kapat
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}


void	handle_parent_process(t_mng_heredocs *mng, int *fd, int j, int *k)
{
	close(fd[1]);
	wait(NULL);
	if (*k < mng->heredoc_nums[j] - 1)
	{
		close(fd[0]);
		(*k)++;
	}
	else
	{
		if (mng->heredoc_flags[j])
		{
			mng->heredoc_fds[j] = fd[0];
		}
		else
		{
			close(fd[0]);
		}
		*k = 0;
	}
}


void	create_pipe_or_exit(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		ft_error();
	}
}

void	fork_or_exit(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		ft_error();
	}
}

void	heredoc_handle(t_mng_heredocs *mng, int heredoc_count)
{
	(void)heredoc_count;
	int	fd[2];
	pid_t	pid;
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	int count = 0;
	int h_count = 0;


	while (mng->heredoc_nums[i] != -1 )
	{
		count++;
		i++;
	}
	i = 0;
	while(i < count)
	{
		j = 0;
		while (j < mng->heredoc_nums[i])
		{
			create_pipe_or_exit(fd);
			fork_or_exit(&pid);
			if (pid == 0)
				handle_child_process(mng->heredoc_delims[h_count], fd[1]);
			else
			{
				handle_parent_process(mng, fd, i, &k);
			}
			j++;
			h_count++;
		}
		i++;
	}
}


