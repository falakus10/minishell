/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falakus <falakus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:02:13 by falakus           #+#    #+#             */
/*   Updated: 2025/08/14 17:03:23 by falakus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	exit(130);
}

static void	ctrl_d(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 19);
	rl_redisplay();
}

static void	ctrl_c(int sig)
{
	(void)sig;
	if (g_signal == 2)
	{
		write(1, "^C\n", 1);
	}
	else if (g_signal == 3)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signal = 130;
}

void	handle_signal(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	setter_signal(int sig)
{
	if (sig == 0)
	{
		signal(SIGQUIT, ctrl_d);
	}
	if (sig == 1)
	{
		signal(SIGQUIT, SIG_IGN);
	}
}
