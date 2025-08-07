#include "minishell.h"

void	close_fd(int input_fd, int output_fd, int index, t_executor *exe)
{
	int	i;
	int	fd_count;
	int used_in;
	int used_out;

	i = 0;
	fd_count = 2 * (exe->count - 1);
	used_in = -1;
	used_out = -1;

	if (index == -1)
	{
		if (input_fd > 2)
			close(input_fd);
		if (output_fd > 2)
			close(output_fd);
	}
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
			if (tmp->file_err || tmp->cmd_err || tmp->path_err || cmd->wrong_path)
			{
				close_fd(tmp->input_fd, tmp->output_fd, i, exe); // Pipe'ları kapat
				exit (1);  //sonra değişcez Başarılı gibi çık
			}
			make_dup(tmp, i, count, exe);
			close_fd(tmp->input_fd, tmp->output_fd, i, exe);
			if (is_builtin(tmp->command))
				exit(built_in(tmp, &exe->env));
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


int multiple_exec(t_command_block *cmd, char **env, t_executor *exe)
{
	int				i;
	t_command_block *tmp;
	int				cmd_count;
	int				last_status;
	int				flag;

	cmd_count = cmd->cmd_count;
	tmp = cmd;
	i = 0;
	flag = 0;
	create_pipe(tmp, exe);
	if (child_exec(cmd, env, cmd_count, exe) != 0)
		return (1);//exit value buraya mı eklenmeli?????
	close_fd(cmd->input_fd, tmp->output_fd, -1, exe);
	while (i < cmd_count)
	{
		if (tmp->next == NULL && tmp->last_fault)
			flag = 1;
		waitpid(tmp->pid, &tmp->status, 0);
		if (i == cmd_count - 1)
			last_status = tmp->status;
		tmp = tmp->next;
		i++;
	}
	if (!flag)
		exe->exp->exit_value = (last_status >> 8) & 0xFF;
	return (0);
}
