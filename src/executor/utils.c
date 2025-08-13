#include "minishell.h"

char	**env_list_to_envp(t_env **env_list)
{
	t_env	*tmp;
	char	**envp;
	int		i;
	int		count;

	tmp = *env_list;
	count = 0;
	while (tmp && ++count)
		tmp = tmp->next;
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		ft_error();
	i = 0;
	tmp = *env_list;
	while (tmp)
	{
		if (*tmp->line)
			envp[i++] = ft_strdup(tmp->line);
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	ft_wait(int pid, t_executor *exe)
{
	int status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		return (1);
	}
	if (WIFSIGNALED(status))
		exe->exp->exit_value = 128 + WTERMSIG(status);
	else
		exe->exp->exit_value = (status >> 8) & 0xFF;
	return (0);
}

void close_unused_fds(int fd_count, int used_in, int used_out, t_executor *exe)
{
	int	i;

	i = 0;
	while (i < fd_count)
	{
		if (i != used_in && i != used_out)
			close(exe->fd[i]);
		i++;
	}
}

void close_in_out_fds(int input_fd, int output_fd)
{
	if (input_fd > 2)
		close(input_fd);
	if (output_fd > 2)
		close(output_fd);
}
