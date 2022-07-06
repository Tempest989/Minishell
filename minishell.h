#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <unistd.h>


typedef struct s_data
{
	// char	*bin;
	char	**command;
	// int		pipe[2];
	// char	flags[5];
}	t_data;

int	ft_split(t_data *data, char *input);

#endif
