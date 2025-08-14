/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:22:37 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:22:38 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	ft_exit(t_command_block *cmd, t_init *init, char **envp)
{
	int	exit_code;

	printf("exit\n");
	exit_code = 2;
	if (!cmd->args[1])
		exit_code = 0;
	else if (!cmd->args[2])
	{
		if (is_number(cmd->args[1]))
			exit_code = ft_atoi(cmd->args[1]) % 256;
		else
			write(2, "minishell: exit: numeric argument required\n", 43);
	}
	else if (is_number(cmd->args[1]))
		return (write(2, "minishell: exit: too many arguments\n", 36), 1);
	else
		write(2, "minishell: exit: numeric argument required\n", 43);
	init->exit_flag = 1;
	free_arr(envp);
	free_all(init);
	exit(exit_code);
}
