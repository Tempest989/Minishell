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

int	ft_left_arrows(t_data *data)
{
	int	track;

	track = 0;
	while (data->command[track] != NULL && ft_strcmp(data->command[track], "<") == 'n')
		track++;
	if (data->command[track] != NULL && track != 0 && (data->is_pipe == -1 || (data->is_pipe != -1 && track < data->is_pipe)))
	{
		data->in_out_fd[4] = dup(0);
		data->in_out_fd[0] = open(data->command[track + 1], O_RDONLY);
		if (data->in_out_fd[0] == -1)
		{
			dprintf(2, "open errro <\n");
			return (-1);
		}
		data->in_out_fd[2] = track;
		dprintf(2, "str = %s\n", data->command[track]);
	}
	else if (track == 0 || (data->is_pipe != -1 && track > data->is_pipe))
	{
		dprintf(2, "first left error track = %d\n", track);
		return (0);
	}
	track = 0;
	while (data->command[track] != NULL && ft_strcmp(data->command[track], "<<") == 'n')
		track++;
	if (data->command[track] != NULL && track != 0 && data->in_out_fd[0] == -1 && (data->is_pipe == -1 || (data->is_pipe != -1 && track < data->is_pipe)))
	{
		// data->in_out_fd[0] = open(data->command[track + 1], O_RDWR);
		// if (data->in_out_fd[0] == -1)
		// {
		// 	dprintf(2, "open errro >\n");
		// 	return (-2);
		// }
		data->in_out_fd[2] = track;
		dprintf(2, "str = %s\n", data->command[track]);
		data->in_out_fd[4] = dup(0);
		if (ft_double_left_arrow(data) < 0)
		{
			dprintf(2, "double left doing error\n");
			return (-1);
		}
	}
	else if (track == 0 || (data->in_out_fd[0] != -1 && data->command[track] != NULL) || (data->is_pipe != -1 && track > data->is_pipe))
	{
		dprintf(2, "double left error\n");
		return (0);
	}
	return (1);
}

int	ft_right_arrows(t_data *data)
{
	int	track;

	track = 0;
	while (data->command[track] != NULL && ft_strcmp(data->command[track], ">") == 'n')
		track++;
	if (data->command[track] != NULL && track != 0 && track > data->is_pipe && track > data->in_out_fd[2])
	{
		data->in_out_fd[5] = dup(1);
		data->in_out_fd[1] = open(data->command[track + 1], O_WRONLY | O_CREAT, 0644);
		if (data->in_out_fd[1] == -1)
		{
			dprintf(2, "open errro >\n");
			return (-1);
		}
		data->in_out_fd[3] = track;
	}
	else if (track == 0 || (data->is_pipe != -1 && track < data->is_pipe) || track < data->in_out_fd[2])
	{
		dprintf(2, "first right error\n");
		return (0);
	}
	track = 0;
	while (data->command[track] != NULL && ft_strcmp(data->command[track], ">>") == 'n')
		track++;
	if (data->command[track] != NULL && track != 0 && data->in_out_fd[1] == -1 && track > data->is_pipe)
	{
		data->in_out_fd[5] = dup(1);
		data->in_out_fd[1] = open(data->command[track + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (data->in_out_fd[1] == -1)
		{
			dprintf(2, "open errro >>\n");
			return (-1);
		}
		data->in_out_fd[3] = track;
	}
	else if (track == 0 || (data->in_out_fd[1] != -1 && data->command[track] != NULL) || (data->is_pipe != -1 && track < data->is_pipe) || track < data->in_out_fd[2])
	{
		dprintf(2, "double right error\n");
		return (0);
	}
	return (1);
}

int	ft_analyse_input(t_data *data)
{
	int	track;

	track = 0;
	while (data->command[track] != NULL && ft_strcmp(data->command[track], "|") == 'n')
		track++;
	if (data->command[track] != NULL && track != 0)
		data->is_pipe = track;
	else if (track == 0)
	{
		dprintf(2, "pipe error\n");
		return (0);
	}
	int check = ft_left_arrows(data);
	if (check == 0)
		return (0);
	else if (check == -1)
		return (-1);
	check = ft_right_arrows(data);
	if (check == 0)
		return (0);
	else if (check == -1)
		return (-1);
	if (data->in_out_fd[0] != -1 && dup2(data->in_out_fd[0], 0) < 0)
	{
		dprintf(2, "dup2 error\n");
		return (-1);
	}
	if (data->in_out_fd[1] != -1 && dup2(data->in_out_fd[1], 1) < 0)
	{
		dprintf(2, "dup2 error\n");
		return (-1);
	}
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
	char	current_dir[PATH_MAX + 2];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd() error");
		return (-1);
	}
	int track = 0;
	while (current_dir[track])
		track++;
	current_dir[track] = ':';
	current_dir[track + 1] = ' ';
	current_dir[track + 2] = '\0';
	line = readline(current_dir);
	if (line[0] == '\0')
	{
		dprintf(2, "line empty\n");
		free(line);
		return (0);
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
