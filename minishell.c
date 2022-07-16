#include "minishell.h"

void	ft_data_destructor(t_data *data, int flag, char *message)
{
	int	track;

	if (data->pipe_locale != NULL)
		free(data->pipe_locale);
	track = 0;
	while (data->command != NULL && data->command[track] != NULL)
		free(data->command[track++]);
	if (data->command != NULL)
		free(data->command);
	if (message != NULL)
	{
		track = 0;
		while (message[track++] != '\0');
		write(2, message, track);
	}
	if (flag == -2)
		return ;
	else if (flag == -1)
	{
		perror("Minishell: ");
		exit(errno);
	}
	else
		exit(flag);
}

void	ft_close_files(t_data *data)
{
	if (data->in_out_fd[0] != -1)
	{
		close(data->in_out_fd[0]);
		if (dup2(data->in_out_fd[4], 0) < 0)
			ft_data_destructor(data, -1, NULL);
	}
	if (data->in_out_fd[1] != -1)
	{
		close(data->in_out_fd[1]);
		if (dup2(data->in_out_fd[5], 1) < 0)
			ft_data_destructor(data, -1, NULL);
	}
	if (data->in_out_fd[0] != -1 && ft_strcmp(data->command[data->in_out_fd[2]], "<<") == 'y')
	{
		if (unlink(".temp") < 0)
			ft_data_destructor(data, -1, NULL);
	}
}

void	ft_initialize(t_data *data)
{
	data->is_pipe = 0;
	data->pipe_locale = NULL;
	data->command_num = 0;
	data->command = NULL;
	data->in_out_fd[0] = -1;
	data->in_out_fd[1] = -1;
	data->in_out_fd[2] = -1;
	data->in_out_fd[3] = -1;
	data->in_out_fd[4] = -1;
	data->in_out_fd[5] = -1;
}

int	main(int result, char *av[], char *ev[])
{
	t_data	data;

	av[0] = NULL;
	while (1)
	{
		ft_initialize(&data);
		result = ft_reading(&data);
		if (result > 0)
			result = ft_execute(&data, ev);
		if (result == -1)
			break ;
		else if (result > 0)
		{
			ft_close_files(&data);
			ft_data_destructor(&data, -2, NULL);
		}
	}
	// dprintf(2, "before destructor\n");
	ft_data_destructor(&data, -2, NULL);
	// dprintf(2, "here\n");
	rl_clear_history();
	return (0);
}
