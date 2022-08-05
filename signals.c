#include "minishell.h"

void	ctrl_c_handler(int sig_num)
{
	write(1, "\n./: ", 5);
	(void)sig_num;
}

void	ctrl_back_slash_hander(int sig_num)
{
	(void)sig_num;
}