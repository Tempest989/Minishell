#include "minishell.h"

int	ft_double_left_arrow(t_data *data)
{
	int		track;
	char	*input;

	data->in_out_fd[0] = open(".temp", O_WRONLY | O_CREAT, 0644);
	if (data->in_out_fd[0] < 0)
		ft_data_destructor(data, -1, NULL);
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(input, data->command[data->in_out_fd[2] + 1]) == 'y')
		{
			free(input);
			break ;
		}
		track = 0;
		while (input[track])
			track++;
		input[track] = '\n';
		write(data->in_out_fd[0], input, track + 1);
		free(input);
	}
	close(data->in_out_fd[0]);
	data->in_out_fd[0] = open(".temp", O_RDONLY);
	if (data->in_out_fd[0] < 0)
		ft_data_destructor(data, -1, NULL);
	return (1);
}