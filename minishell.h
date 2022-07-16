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


typedef struct s_data
{
	// char	*bin;
	char	**command;
	// int		pipe[2];
	int	is_pipe;
	int	*pipe_locale;
	int	in_out_fd[6];
	int	command_num;
}	t_data;

int		ft_split(t_data *data, char *input);
int		ft_reading(t_data *data);
int		ft_execute(t_data *data, char *ev[]);
char	ft_strcmp(const char *str1, const char *str2);
int		ft_double_left_arrow(t_data *data);
int		ft_bin_path(t_data *data, char *ev[], int position);
void	ft_data_destructor(t_data *data, int flag, char *message);

#endif
