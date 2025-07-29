#include "minishell.h"

void	close_fd(int index, int count, t_executor *exe)
{
	int	i;
	int	fd_count;
	int used_in;
	int used_out;

	i = 0;
	fd_count = 2 * (count - 1);
	used_in = -1;
	used_out = -1;

	if (index == 0)
		used_out = 1;
	else if (index == count - 1)
		used_in = 2 * (index - 1);
	else
	{
		used_in = 2 * (index - 1);
		used_out = 2 * index + 1;
	}
	while (i < fd_count)
	{
		if (i != used_in && i != used_out)
			close(exe->fd[i]);
		i++;
	}
}
int	child_exec(t_command_block *cmd, char **env, int count, t_executor *exe)
{
	t_command_block	*tmp;
	int	i;

	i = 0;
	tmp = cmd;
	while (i < count)
	{
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			make_dup(tmp, i, count, exe);
			close_fd(i, count, exe);
			if (is_builtin(tmp->command))
				exit(built_in(cmd, env, exe));
			execve(tmp->command, tmp->args, env);
			perror("execve");
			exit(1);
		}
		else if (tmp->pid < 0)
		{
			perror("fork faild");
			return (1);
		}
		i++;
		tmp = tmp->next;
	}
	return (0);
}


int multiple_exec(t_command_block *cmd, t_env *env, t_executor *exe)
{
	int				i;
	t_command_block *tmp;
	int				cmd_count;
	char			**envp;
	
	envp = env_list_to_envp(env);
	cmd_count = cmd->cmd_count;
	tmp = cmd;
	i = 0;
	create_pipe(tmp, exe);
	if (child_exec(cmd, env, cmd_count, exe) != 0)
		return (1);
	close_fd(-1, cmd_count, exe);
	while (i < cmd_count)
	{
		waitpid(tmp->pid, &tmp->status, 0);
		tmp->last_output = (tmp->status >> 8) & 0xFF;
		tmp = tmp->next;
		i++;
	}
	return (0);
}
