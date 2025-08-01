#include "minishell.h"

/*void	heredoc_handle(t_mng_heredocs *mng, int heredoc_count)
{
    int		fd[2];
	pid_t	pid;
	char	*line;
	int		i;
	int		j;
	int		k;

    i = 0;
    j = 0;
    k = 0;
	while(i < heredoc_count)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) // child process
		{
			close(fd[0]); // read ucunu kapat

			while (1)
			{
				line = readline("> ");
				if (!line || ft_strcmp(line, mng->heredoc_delims[i]) == 0)
				{
					free(line);
					break;
				}
				write(fd[1], line, ft_strlen(line));
				write(fd[1], "\n", 1);
				free(line);
			}
			close(fd[1]); // write ucunu kapat
			exit(0);
		}
		else // parent process
		{
			close(fd[1]); // write ucunu kapat
			waitpid(pid, NULL, 0);

			if (k < mng->heredoc_nums[j] -1)
			{
				close(fd[0]);
				k++;
			}	
			else
			{
				if (mng->heredoc_flags[j])
				{
					mng->heredoc_fds[j] = fd[0];
					printf("buraya kaç kere girdim\n");
				}
				else
					close(fd[0]); // kullanılmayacaksa kapat
				k = 0;
				j++;
			}
		}
    i++;
	}
}*/

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


void	handle_parent_process(t_mng_heredocs *mng, int *fd, int *j, int *k)
{
	close(fd[1]);
	wait(NULL);

	if (*k < mng->heredoc_nums[*j] - 1)
	{
		close(fd[0]);
		(*k)++;
	}
	else
	{
		if (mng->heredoc_flags[*j])
			mng->heredoc_fds[*j] = fd[0];
		else
			close(fd[0]);
		*k = 0;
		(*j)++;
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
	int	fd[2];
	pid_t	pid;
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (i < heredoc_count)
	{
		create_pipe_or_exit(fd);
		fork_or_exit(&pid);
		if (pid == 0)
			handle_child_process(mng->heredoc_delims[i], fd[1]);
		else
			handle_parent_process(mng, fd, &j, &k);
		i++;
	}
}
