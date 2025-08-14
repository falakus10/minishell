/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:22:23 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 14:22:24 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_a_built(t_command_block *cmd, t_env **env, t_init *init, char **enp)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		perror("dup");
	make_dup(cmd, 0, 1, init->exec);
	built_in(cmd, env, init, enp);
	if (dup2(saved_stdin, STDIN_FILENO) == -1
		|| dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("dup2 restore");
	close(saved_stdin);
	close(saved_stdout);
}

int	built_in(t_command_block *cmd, t_env **env, t_init *init, char **envp)
{
	int	which_cmd;

	if (cmd->file_err || cmd->cmd_err)
		return (1);
	which_cmd = is_builtin(cmd->command);
	if (which_cmd == 1)
		cmd->expnd->exit_value = ft_cd(cmd, *env);
	else if (which_cmd == 2)
		cmd->expnd->exit_value = ft_export(cmd, *env);
	else if (which_cmd == 3)
		cmd->expnd->exit_value = ft_unset(cmd, env);
	else if (which_cmd == 4)
		cmd->expnd->exit_value = ft_exit(cmd, init, envp);
	else if (which_cmd == 5)
		cmd->expnd->exit_value = ft_echo(cmd);
	else if (which_cmd == 6)
		cmd->expnd->exit_value = ft_pwd();
	else if (which_cmd == 7)
		cmd->expnd->exit_value = ft_env(*env);
	else
		return (-1);
	free_arr(envp);
	return (cmd->expnd->exit_value);
}
