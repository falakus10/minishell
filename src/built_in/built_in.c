#include "minishell.h"

void	run_single_builtin(t_command_block *cmd, t_executor *exe, t_env **env, t_init *init)
{
	int saved_stdin;
	int saved_stdout;


	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		perror("dup");
	make_dup(cmd, 0, 1, exe);
	built_in(cmd, env, init);
	if (dup2(saved_stdin, STDIN_FILENO) == -1 ||
		dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("dup2 restore");
	close(saved_stdin);
	close(saved_stdout);
}

int	built_in(t_command_block *cmd, t_env **env, t_init *init)
{
	int	which_cmd;
	int value;

	if (cmd->file_err || cmd->cmd_err) //path le ilgili error ler buraya gelmez çünkü işi yok path  le
		return (1);
	value = 0;
	which_cmd = is_builtin(cmd->command);
	if (which_cmd == 1)
		value = ft_cd(cmd, *env);
	else if (which_cmd == 2)
		value = ft_export(cmd, *env);
	else if (which_cmd == 3)
		value = ft_unset(cmd, env);
	else if (which_cmd == 4)
		value = ft_exit(cmd, init);
	else if (which_cmd == 5)
		value = ft_echo(cmd);
	else if (which_cmd == 6)
		value = ft_pwd();
	else if (which_cmd == 7)
		value = ft_env(*env);
	else
		return (-1);
	cmd->expnd->exit_value = value;
	return (value);
}
