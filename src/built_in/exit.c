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

int	ft_exit(t_command_block *cmd, t_init *init)
{
	int				exit_code;

	printf("exit\n");
	exit_code = 2;
	if (cmd->args[1] == NULL)
		exit_code = 0;
	if (cmd->args[1] != NULL && (cmd->args[2] == NULL))
	{
		if (is_number(cmd->args[1]))
			exit_code = (ft_atoi(cmd->args[1])) % 256;
		else
			write(2, "minishell: exit: numeric argument required\n", 43);
	}
	else if (cmd->args[1] && cmd->args[2])
	{
		if (is_number(cmd->args[1]))
		{
			write(2, "minishell: exit: too many arguments\n", 36);//hata mesajı güncelle(kontrol et)
				return (1);
		}
		else
			write(2, "minishell: exit: numeric argument required\n", 43);
	}
	init->exit_flag = 1;
	free_all(init); //env_list'i de freele
	exit (exit_code);
}
