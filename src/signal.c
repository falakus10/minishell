#include "minishell.h"

void	sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == 2)
	{
		write(1, "\n", 1);      // imleci alt satıra al
		rl_on_new_line();       // readline'a "yeni satır" olduğunu bildir
		rl_replace_line("", 0); // yazılmış satırı temizle
		rl_redisplay();         // prompt'u tekrar göster
	}
	else
		return ;
}

static void	suppress_output(void)
{
	struct termios termios_p;     // 1. Terminal ayarlarını tutacak yapı tanımlanıyor
	if (tcgetattr(0, &termios_p) != 0)	//Standart girişin (fd=0) mevcut terminal ayarlarını alıyor
		perror("Minishell: tcgetattr");	// Eğer başarısız olursa hata mesajı yazdırıyor
	termios_p.c_lflag &= ~ECHOCTL;		// 3. Terminal yerel ayar bayraklarından ECHOCTL bayrağını kapatıyor(Bu, Ctrl+C gibi kontrol karakterlerinin ^C şeklinde ekranda görünmesini engeller)
	if (tcsetattr(0, 0, &termios_p) != 0)	// 4. Güncellenmiş terminal ayarlarını hemen (TCSANOW) standart girişe uyguluyor
		perror("Minishell: tcsetattr");		// Eğer başarısız olursa hata mesajı yazdırıyor
} 

void	signal_handler(void)
{
	struct sigaction	sa_int;

	suppress_output();
	sa_int.sa_sigaction = sigint_handler;
	sa_int.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}
