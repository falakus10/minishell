#include "minishell.h"

int	new_line_check(t_command_block	*cmd)
{
	int i;
	int	j;

	i = 1;
	while (cmd->args[i] && !(ft_strncmp(cmd->args[i], "-n", 2)))
	{
		j = 2;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] != '\0')
			break;
		i++;
	}
	if (i != 1)
		return (i);
	return (1);
}

int	ft_echo(t_command_block *cmd)
{
	int i;
	int	new_line;

	i = 1;
	new_line = 1;
	if (cmd->args[1] == NULL)
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		if(new_line_check(cmd) > 1)
		{
			i = new_line_check(cmd);
			new_line = 0;
		}
		while (cmd->args[i])
		{
			write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
			if (cmd->args[i + 1] != NULL)
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
	}
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
