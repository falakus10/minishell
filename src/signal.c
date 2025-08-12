#include "minishell.h"

void sigint_handler(int sig)
{
    if (sig == 0)  // genelde 2'nin karşılığı SIGINT
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
	else if(sig == 1) //heredoc içinde Ctrl+C basıldığında
	{

	}

}

void sigquit_handler(int sig)
{
    (void)sig; // kullanılmayan parametre uyarısı için
    // İstersen buraya quit sinyali için işlem ekle
}

void setup_signal_handlers(void)
{
    struct sigaction sa_int; //SIGINT için yapı
	struct sigaction sa_quit;//SIGQUIT için yapı

	sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
	
	sa_quit.sa_handler = sigquit_handler;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

