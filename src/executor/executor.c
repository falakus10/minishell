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
	if (cmd->file_err || cmd->cmd_err || cmd->path_err) //DÜZENLE
		return (1);
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		make_dup(cmd, 0, count, exe);
		execve(cmd->command, cmd->args, env);
		perror("execve failed!");
		exit (1);
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
		exe->exp->exit_value = (cmd->status >> 8) & 0xFF;
	}
	return (0);
}


int	executor(t_command_block *cmd, t_env *env, t_executor *exe)
{
	char  **envp;

	envp = env_list_to_envp(env);
	cmd->cmd_count = command_count(cmd);
	exe->count = cmd->cmd_count;
	if (cmd->cmd_count == 1)
	{
		if (is_builtin(cmd->command))
		{	
			run_single_builtin(cmd, exe);
		}
		else
			run_single_cmd(cmd, envp, cmd->cmd_count, exe);
	}
	else
	{
		multiple_exec(cmd, envp, exe);
	}
	return (0);
}

