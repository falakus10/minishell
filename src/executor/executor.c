#include "minishell.h"

void	create_pipe(t_command_block *cmd, t_executor *exe)
{
	int	i;

	i = 0;
	exe->fd = malloc(sizeof(int) * 2 * (cmd->cmd_count - 1));
	if (exe->fd == NULL)
	{
		perror("malloc");
		exit(1);
	}
	while (i < cmd->cmd_count - 1)
	{
		if(pipe(&exe->fd[2 * i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	make_dup(t_command_block *cmd, int index, int count, t_executor *exe)
{
	if (cmd->input_fd != -3)
		dup2(cmd->input_fd, STDIN_FILENO);
	if (cmd->output_fd != -3)
		dup2(cmd->output_fd, STDOUT_FILENO);
	if (cmd->output_fd == -3)
	{
		if (count == 1)
			return ;
		if (index == 0)
			dup2(exe->fd[1], STDOUT_FILENO);
		else if (index != count - 1)
			dup2(exe->fd[2 * index + 1], STDOUT_FILENO);
	}
	if (cmd->input_fd == -3)
	{
		if (index != 0)
			dup2(exe->fd[2 * (index - 1)], STDIN_FILENO);
	}
}

int command_count(t_command_block *cmd)
{
	int				count;
	t_command_block	*temp;

	count = 0;
	temp = cmd;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

int	run_single_cmd(t_command_block *cmd, char **env, int count, t_executor *exe)
{
	if (cmd->file_err || cmd->cmd_err) //DÜZENLE
		return (1);
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		make_dup(cmd, 0, count, exe);
		if (is_builtin(cmd->command))//CHİLD DA ÇALIŞMASIN
		{
			exe->exp->exit_value = built_in(cmd, exe->env);
			exit (exe->exp->exit_value);
		}
		else
		{
			execve(cmd->command, cmd->args, env);//char ** alıcak;
			perror("execve failed!");
			exit (1);
		}
	}
	else if (cmd->pid < 0)
	{
		perror("fork failed!");
		return (1);
	}
	else
	{
		if(waitpid(cmd->pid, &cmd->status, 0) == -1)
		{
			perror("waitpid failed!");
			return (1);
		}
		cmd->last_output = (cmd->status >> 8) & 0xFF;
	}
	return (0);
}


int	executor(t_command_block *cmd, char **env, t_executor *exe)
{
	cmd->cmd_count = command_count(cmd);
	if (cmd->cmd_count == 1)
	{
		run_single_cmd(cmd, env, cmd->cmd_count, exe);
	}
	else
	{
		multiple_exec(cmd, env, exe);
	}
	return (0);
}

