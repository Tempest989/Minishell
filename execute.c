#include "minishell.h"

int	ft_piping(t_data *data, char *ev[])
{
	pid_t	pid;
	int		fd[2][2];
	int		status;
	int		start;

	for (int i = 0; i < 2; i++)
	{
		ft_update_start(data, &start);
		if (pipe(fd[i]) == -1)
		{
			dprintf(2, "pipes error pipe\n");
			return (-1);
		}
		pid = fork();
		if (pid < 0)
		{
			dprintf(2, "fork error pipe\n");
			return (-1);
		}
		if (pid > 0)
		{
			close(fd[i][1]);
			if (dup2(fd[i][0], 0) == -1)
			{
				dprintf(2, "dup2 error pipe\n");
				return (-1);
			}
			pid = waitpid(0, &status, WNOHANG);
			if (pid == -1)
			{
				dprintf(2, "pid error pipe\n");
				return (-1);
			}
		}
		else
		{
			close(fd[i][0]);
			if (dup2(fd[i][1], 1) == -1)
			{
				dprintf(2, "duo2 error pipe\n");
				return (-1);
			}
			ft_exe(av, ev, start);
		}
	}
	return (1);
}

void	ft_exe(t_data *data, char *ev[], int start)
{
	char	**output;
	int		track;

	track = 0;
	while (track + start <= data->command_num && track + start != data->is_pipe && track + start != data->in_out_fd[2] && track + start != data->in_out_fd[3])
		track++;
	output = malloc(sizeof(char *) * (track + 1));
	if (output == NULL)
	{
		dprintf(2, "malloc erorr exe\n");
		exit(2);
	}
	track = 0;
	while (track + start <= data->command_num && track + start != data->is_pipe && track + start != data->in_out_fd[2] && track + start != data->in_out_fd[3])
	{
		output[track] = data->command[track + start];
		track++;
	}
	output[track] = NULL;
	execve(output[0], output, ev);
	//do error stuff etc
	exit(127);
}

int	ft_execute(t_data *data, char *ev[])
{
	if (data->is_pipe == -1)
		return (ft_piping(data, ev));
	pid = fork();
	if (pid < 0)
	{
		dprintf(2, "fork error\n");
		return (-1);
	}
	if (pid > 0)
	{
		pid = waitpid(0, &status, WNOHANG);
		if (pid == -1)
			ft_error_return(NULL, 0);
	}
	else
		ft_exe(data, ev, 0);
	return (1);
}
