#include "minishell.h"

int	ft_cd(t_command_block *cmd, char **env, t_executor *exe)
{
	
}



int	built_in(t_command_block *cmd, char **env, t_executor *exe)
{
	int	which_cmd;
	int value;

	value = 0;
	which_cmd = is_builtin(cmd->command);
	if (which_cmd == 1)
		value = ft_cd(cmd, env, exe);
	else if (which_cmd == 2)
		value = ft_export(cmd, env, exe);
	else if (which_cmd == 3)
		value = ft_unset(cmd, env, exe);
	else if (which_cmd == 4)
		value = ft_exit(cmd, env, exe);
	else if (which_cmd == 5)
		value = ft_echo(cmd, env, exe);
	else if (which_cmd == 6)
		value = ft_pwd(cmd, env, exe);
	else if (which_cmd == 7)
		value = ft_env(cmd, env, exe);
	else
		return (-1);
	return (value);
}
