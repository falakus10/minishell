#include "minishell.h"

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

int	run_single_cmd(t_command_block *cmd, char **env, t_expander *exp)
{
	pid_t	pid;

	pid = fork();
	dups(cmd, -1);
	if (pid == 0)
	{
		execve(cmd->command, cmd->args, env);
		perror("execve failed!");
		exit (1);
	}
	else if (pid < 0)
	{
		perror("fork failed!");
		return (1);
	}
	else
	{
		if(waitpid(pid, &exp->last_output, 0) == -1)
		{
			perror("waitpid failed!");
			return (1);
		}
		exp->last_output = (exp->last_output) >> 8 & 0xFF;
	}
	return (0);
}

void	dups(t_command_block *cmd, int index)
{
	if (cmd->input_fd > 2)
		dup2(cmd->input_fd, STDIN_FILENO);
	if (cmd->output_fd > 2)
		dup2(cmd->output_fd, STDOUT_FILENO);
	if (cmd->output_fd == -1 && cmd->input_fd == -1)
	{
		if (index == 0)
			dup2(cmd->fd[0], STDOUT_FILENO);
		else if (index == cmd->cmd_count - 1)
			dup2(cmd->fd[index - 1], STDIN_FILENO);
		else
		{
			dup2(cmd->fd[2 * index + 1], STDOUT_FILENO);
			dup2(cmd->fd[2 * (index - 1)], STDIN_FILENO);
		}
	}
}

int	executor(t_command_block *cmd, char **env, t_expander *exp)
{
	cmd->cmd_count = command_count(cmd);
	if (cmd->cmd_count == 1)
	{
		if (is_builtin(cmd->command))
		{
			//built in çalışıtr
		}
		else
		{
			run_single_cmd(cmd, env, exp);
		}
	}
	else
	{
		//çok komut çalışıtr
		//her birini built in mi kontrol et
	}
}

