#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	exit(130);
}

static void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p) != 0)
		perror("Minishell: tcgetattr");
	if (tcsetattr(0, 0, &termios_p) != 0)
		perror("Minishell: tcsetattr");
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
		write(1, "\n",1);  // ^C bastır
	}
	else if (g_signal == 3)
	{
		write(1, "\n",1);  // ^C bastır
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
	{
		write(1, "\n", 1);  // ^C bastır
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signal = 130;
}
/* static void	ctrl_c2(int sig)
{
	(void)sig;
	if (g_signal == 2)
	{
		write(1, "^C\n",3);  // ^C bastır
	}
	else if (g_signal == 3)
	{
		write(1, "\n",1);  // ^C bastır
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
	{
		write(1, "\n", 1);  // ^C bastır
		rl_on_new_line();
		rl_replace_line("", 0);
		//rl_redisplay();
	}
	g_signal = 130;
} */

void	handle_signal(void)
{
	suppress_output();
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void setter_signal(int sig)
{
	if (sig == 0)
	{
		signal(SIGQUIT, ctrl_d);
		//signal(SIGINT, ctrl_c2);
	}
	if (sig == 1)
	{
		signal(SIGQUIT, SIG_IGN);
		//signal(SIGINT, ctrl_c);
	}
}
