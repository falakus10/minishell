#include "minishell.h"

void close_old_fd(int old_fd)
{
	if (old_fd > -1 && old_fd != -3)
		close(old_fd);
}