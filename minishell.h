#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>


typedef struct s_data
{
	// char	*bin;
	char	**command;
	// int		pipe[2];
	int	is_pipe;
	int	in_out_fd[6];
	int	command_num;
}	t_data;

int	ft_split(t_data *data, char *input);
int	ft_reading(t_data *data);
char	ft_strcmp(const char *str1, const char *str2);
int	ft_double_left_arrow(t_data *data);

#endif
