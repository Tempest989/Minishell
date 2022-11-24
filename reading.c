#include "minishell.h"

char	ft_strcmp(const char *str1, const char *str2)
{
	int	track;

	track = 0;
	while (str1[track] && str2[track] && str1[track] == str2[track])
		track++;
	if (str1[track] == '\0' && str2[track] == '\0')
		return ('y');
	return ('n');
}

int	ft_arrow_loop(t_data *data, char *first, char *second, int flag)
{
	int	track;

	track = 0;
	while (data->command[track] != NULL)
	{
		if (ft_strcmp(data->command[track], first) == 'y' || ft_strcmp(data->command[track], second) == 'y')
		{
			if ((flag == 1 && data->in_out_fd[2] != -1) || data->in_out_fd[3] != -1)
			{
				if (flag == 1)
					dprintf(2, "left");
				else
					dprintf(2, "right");
				dprintf(2, " double error track = %d\n", track);
				return (0);
			}
			if (flag == 1)
				data->in_out_fd[2] = track;
			else
				data->in_out_fd[3] = track;
			if (data->pipe_locale != NULL && ((flag == 1 && track > data->pipe_locale[0]) || (flag != 1 && track < data->pipe_locale[data->is_pipe - 1])))
			{
				if (flag == 1)
					dprintf(2, "left");
				else
					dprintf(2, "right");
				dprintf(2, " arrow after pipe\n");
				dprintf(2, "pipe_locale = %d, track = %d\n", data->pipe_locale[0], track);
				return (0);
			}
		}
		track++;
	}
	return (1);
}

int	ft_left_arrows(t_data *data)
{
	if (ft_arrow_loop(data, "<", "<<", 1) == 0)
		return (0);
	if (data->in_out_fd[2] == -1)
		return (1);
	data->in_out_fd[4] = dup(0);
	if (data->in_out_fd[4] < 0)
		ft_data_destructor(data, -1, NULL);
	if (ft_strcmp(data->command[data->in_out_fd[2]], "<") == 'y')
	{
		data->in_out_fd[0] = open(data->command[data->in_out_fd[2] + 1], O_RDONLY);
		if (data->in_out_fd[0] == -1)
			ft_data_destructor(data, -1, NULL);
		dprintf(2, "str = %s\n", data->command[data->in_out_fd[2]]);
	}
	else
	{
		// data->in_out_fd[0] = open(data->command[track + 1], O_RDWR);
		// if (data->in_out_fd[0] == -1)
		// {
		// 	dprintf(2, "open errro >\n");
		// 	return (-2);
		// }
		dprintf(2, "str = %s\n", data->command[data->in_out_fd[2]]);
		if (ft_double_left_arrow(data) < 0)
		{
			dprintf(2, "double left doing error\n");
			return (-1);
		}
	}
	return (1);
}

int	ft_right_arrows(t_data *data)
{
	if (ft_arrow_loop(data, ">", ">>", 0) == 0)
		return (0);
	if (data->in_out_fd[3] == -1)
		return (1);
	data->in_out_fd[5] = dup(1);
	if (data->in_out_fd[5] < 0)
		ft_data_destructor(data, -1, NULL);
	if (ft_strcmp(data->command[data->in_out_fd[3]], ">") == 'y')
		data->in_out_fd[1] = open(data->command[data->in_out_fd[3] + 1], O_WRONLY | O_CREAT, 0644);
	else
		data->in_out_fd[1] = open(data->command[data->in_out_fd[3] + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (data->in_out_fd[1] == -1)
		ft_data_destructor(data, -1, NULL);
	return (1);
}

int	ft_pipe_checks(t_data *data)
{
	int	track;

	track = 0;
	while (data->command[track] != NULL)
	{
		if (ft_strcmp(data->command[track], "|") == 'y')
		{
			if (track == 0)
			{
				dprintf(2, "0 | error\n");
				return (0);
			}
			else if (data->command[track + 1] == NULL)
			{
				dprintf(2, "command not found\n");
				return (0);
			}
			data->is_pipe++;
		}
		track++;
	}
	if (data->is_pipe == 0)
		return (1);
	data->pipe_locale = malloc(sizeof(int) * (data->is_pipe + 1));
	if (data->pipe_locale == NULL)
		ft_data_destructor(data, -1, NULL);
	data->is_pipe = 0;
	track = 0;
	while (data->command[track] != NULL)
	{
		if (ft_strcmp(data->command[track], "|") == 'y')
		{
			data->pipe_locale[data->is_pipe] = track;
			data->is_pipe++;
		}
		track++;
	}
	data->pipe_locale[data->is_pipe] = -1;
	return (1);
}

int	ft_analyse_input(t_data *data)
{
	int	check;

	check = ft_environ_checks(data, NULL);
	if (check != 0)
		return (check);
	check = ft_pipe_checks(data);
	if (check <= 0)
		return (check);
	check = ft_left_arrows(data);
	if (check <= 0)
		return (check);
	check = ft_right_arrows(data);
	if (check <= 0)
		return (check);
	if (data->in_out_fd[0] != -1 && dup2(data->in_out_fd[0], 0) < 0)
		ft_data_destructor(data, -1, NULL);
	if (data->in_out_fd[1] != -1 && dup2(data->in_out_fd[1], 1) < 0)
		ft_data_destructor(data, -1, NULL);
	// if (data->in_out_fd[0] != -1)
	// {
	// 	char temp[5];
	// 	if (read(0, temp, 5) <= 0)
	// 	{
	// 		dprintf(2, "error read\n");
	// 		return (-1);
	// 	}
	// 	write(2, temp, 5);
	// }
	// if (data->in_out_fd[1] != -1)
	// {
	// 	printf("testing\n");
	// }
	return (1);
}

int	ft_reading(t_data *data)
{
	char	*line;
	char	current_dir[PATH_MAX + 3];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd() error");
		return (-1);
	}
	// printf("output = %s\n", current_dir);
	int track = 0;
	while (current_dir[track++]);
	current_dir[track - 1] = ':';
	current_dir[track] = ' ';
	current_dir[track + 1] = '\0';
	// printf("output = %s\n", current_dir);
	// sigaction(SIGINT, data->new[0], data->old[0]);
	// sigaction(SIGQUIT, data->new[1], data->old[1]);
	// sigaction(SIG)										/// put signal shit here, can handle all signals i think, need to detect ctrl+d, then can place SIGINt signal there for new signal
	line = readline(current_dir);
	// track = 0;
	// printf("%s",current_dir);
	// for (int i = 0; i < 101; i++)
	// 	line[i] = 0;
	// fgets(line, 100, stdin);
	// for (int i = 0; line[i] != '\0'; i++)
	// {
	// 	if (line[i] == '\n')
	// 	{
	// 		line[i] = '\0';
	// 		break;
	// 	}
	// }
	// printf("line = :%s:\n", line);
	if (line == NULL || line[0] == '\0')
	{
		dprintf(2, "line empty\n");
		free(line);
		return (0);
	}
	if (ft_strcmp(line, "exit") == 'y')
	{
		dprintf(2, "exiting...\n");
		free(line);
		return (-1);
	}
	data->command_num = ft_split(data, line);
	add_history(line);
	free(line);
	int check = ft_analyse_input(data);
	if (check == 0)
	{
		dprintf(2, "warning: invalid input, please try again\n");
		return (0);
	}
	else if (check < 0)
		return (-1);
	return (1);
}
