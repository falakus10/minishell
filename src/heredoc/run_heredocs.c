#include "minishell.h"

void	handle_child_process(t_mng_heredocs *mng, char *delim, int write_fd, t_init *init)
{
	char *line;

	close(write_fd - 1); // read ucunu kapat
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_DFL);
	init->mng_hrdcs->f_flag = 0; // Çocuk işlemde artık heredoc flag'ini kapatıyoruz
	init->exit_flag = 1;
	free_all(init);
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
	free_mng(mng);
	close(write_fd);
	exit(0);
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
	g_signal = 2;
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		ft_error();
	}
}

int	handle_parent_process(t_mng_heredocs *mng, int *fd, int j, int *k)
{
	int status;
	pid_t child_pid;

	close(fd[1]);
	child_pid = wait(&status);
	if (child_pid == -1)
	{
		perror("wait");
		return (1);
	}
	if (WIFEXITED(status))
	{
		int exit_code = WEXITSTATUS(status);
		if (exit_code == 130)
		{
			close(fd[0]);
			mng->heredoc_flags[j] = 0;
			return (1);
		}
	}
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
	g_signal = 0;
	return (0);
}

int	heredoc_handle(t_mng_heredocs *mng, int heredoc_count, t_init *init)
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


	count = count_cmd_blk(init->jnd_lxr_lst);
	i = 0;
	while(i < count)
	{
		j = 0;
		while (j < mng->heredoc_nums[i])
		{
			create_pipe_or_exit(fd);
			fork_or_exit(&pid);
			if (pid == 0)
			{
				handle_child_process(mng ,mng->heredoc_delims[h_count], fd[1], init);
				mng->f_flag = 1;
			}
			else
			{
				if(handle_parent_process(mng, fd, i, &k))
					return (1);
			}
			j++;
			h_count++;
		}
		i++;
	}
	return (0);
}
