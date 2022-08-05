#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct s_data
{
	char				**command;
	int					is_pipe;
	int					*pipe_locale;
	int					in_out_fd[6];
	int					command_num;
	// struct sigaction	old[3];
	// struct sigaction	new[2];
}	t_data;

int		ft_split(t_data *data, char *input);
int		ft_reading(t_data *data);
int		ft_execute(t_data *data, char *ev[]);
char	ft_strcmp(const char *str1, const char *str2);
int		ft_double_left_arrow(t_data *data);
int		ft_bin_path(t_data *data, char *ev[], int position);
void	ft_data_destructor(t_data *data, int flag, char *message);
void	ctrl_c_handler(int sig_num);
void	ctrl_back_slash_hander(int sig_num);

#endif
