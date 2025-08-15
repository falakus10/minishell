/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:12:53 by austunso          #+#    #+#             */
/*   Updated: 2025/08/15 14:07:47 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child(t_mng_heredocs *mng, char *delim, \
	int write_fd, t_init *init)
{
	char	*line;

	close(write_fd - 1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_DFL);
	init->mng_hrdcs->f_flag = 0;
	init->exit_flag = 1;
	free_all(init);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
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

int	exit_status(t_mng_heredocs *mng, int status, int *fd, int j)
{
	int	exit_code;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code == 130)
		{
			close(fd[0]);
			mng->heredoc_flags[j] = 0;
			return (1);
		}
	}
	return (0);
}

int	handle_parent_process(t_mng_heredocs *mng, int *fd, int j, int *k)
{
	int		status;
	pid_t	child_pid;

	close(fd[1]);
	child_pid = wait(&status);
	if (child_pid == -1)
	{
		perror("wait");
		return (1);
	}
	if (exit_status(mng, status, fd, j))
		return (1);
	if (++(*k) < mng->heredoc_nums[j] - 1)
	close(fd[0]);
	else
	{
		if (mng->heredoc_flags[j])
		mng->heredoc_fds[j] = fd[0];
		else
		close(fd[0]);
		*k = 0;
	}
	g_signal = 0;
	return (0);
}
