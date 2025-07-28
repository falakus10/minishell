#include "minishell.h"

int	is_number(char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(t_command_block *cmd)
{
	int				exit_code;

	printf("exit\n");
	exit_code = 0;
	if (cmd->args[1] != NULL && (cmd->args[2] == NULL))
	{
		if (is_number(cmd->args[1]))
			exit_code = ft_atoi(cmd->args[1]);
		else
			exit_code = 255;
	}
	else if (cmd->args[1] && cmd->args[2])
	{
		if (is_number(cmd->args[1]))
		{
			write(2, "exit: too many arguments\n", 25);//hata mesajı güncelle(kontrol et)
				return (1);
		}
		else
		{
			write(2, "exit: numeric argument required\n", 32);
			exit_code = 255;
		}
	}
	exit (exit_code);
}
