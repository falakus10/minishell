#include "minishell.h"

void	close_fd(int input_fd, int output_fd, int index, t_executor *exe)
{
	int	fd_count;
	int	used_in;
	int	used_out;

	fd_count = 2 * (exe->count - 1);
	used_in = -1;
	used_out = -1;
	if (index == -1)
		close_in_out_fds(input_fd, output_fd);
	else if (index == 0 && output_fd > -1)
		used_out = 1;
	else if (index == exe->count - 1 && input_fd > -1)
		used_in = 2 * (index - 1);
	else
	{
		if (input_fd > -1)
			used_in = 2 * (index - 1);
		if (output_fd > -1)
			used_out = 2 * index + 1;
	}
	close_unused(fd_count, used_in, used_out, exe);
}

void	child(t_command_block *tmp, char **env, t_executor *exe, t_init *init)
{
	int	value;

	value = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (tmp->file_err || tmp->cmd_err || tmp->path_err || tmp->wrong_path)
	{
		init->exit_flag = 1;
		free_all(init);
		close_fd(tmp->input_fd, tmp->output_fd, exe->i, exe);
		exit (1);
	}
	make_dup(tmp, exe->i, exe->count, exe);
	close_fd(tmp->input_fd, tmp->output_fd, exe->i, exe);
	if (is_builtin(tmp->command))
	{
		value = built_in(tmp, &exe->env, init, env);
		init->exit_flag = 1;
		free_all(init);
		exit (value);
	}
	execve(tmp->command, tmp->args, env);
	perror("execve");
	exit(1);
}

int	child_exec(t_command_block *cmd, char **env, t_executor *exe, t_init *init)
{
	t_command_block	*tmp;

	exe->count = command_count(cmd, exe);
	exe->i = 0;
	tmp = cmd;
	while (exe->i < exe->count)
	{
		tmp->pid = fork();
		if (tmp->pid == 0)
			child(tmp, env, exe, init);
		else if (tmp->pid < 0)
		{
			perror("fork faild");
			return (1);
		}
		exe->i++;
		tmp = tmp->next;
	}
	return (0);
}

int	multiple_wait(int cmd_count, t_command_block *tmp, int *flag)
{
	int	i;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (tmp->next == NULL && tmp->last_fault)
			*flag = 1;
		waitpid(tmp->pid, &tmp->status, 0);
		if (i == cmd_count - 1)
			last_status = tmp->status;
		tmp = tmp->next;
		i++;
	}
	return (last_status);
}

int	multi_exec(t_command_block *cmd, char **env, t_executor *exe, t_init *init)
{
	t_command_block	*tmp;
	int				cmd_count;
	int				last_status;
	int				flag;

	cmd_count = cmd->cmd_count;
	tmp = cmd;
	flag = 0;
	g_signal = 2;
	create_pipe(tmp, exe);
	if (child_exec(cmd, env, exe, init) != 0)
		return (1);
	close_fd(cmd->input_fd, tmp->output_fd, -1, exe);
	last_status = multiple_wait(cmd_count, tmp, &flag);
	if (!flag)
	{
		if (WIFSIGNALED(last_status))
			exe->exp->exit_value = 128 + WTERMSIG(last_status);
		else
			exe->exp->exit_value = (last_status >> 8) & 0xFF;
	}
	return (0);
}
