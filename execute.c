#include "minishell.h"

void	ft_exe(t_data *data, char *ev[], int start)
{
	char	**output;
	int		track;

	// dprintf(2, "enetered exe\n");
	if (data->is_pipe != 0)
		start = data->pipe_locale[start] + 1;
	while (data->command[start] != NULL && (ft_strcmp(data->command[start], "<") == 'y' || ft_strcmp(data->command[start], "<<") == 'y' || ft_strcmp(data->command[start], ">") == 'y' || ft_strcmp(data->command[start], ">>") == 'y' || ft_strcmp(data->command[start], "|") == 'y'))
		start += 2;
	if (data->command[start] == NULL)
	{
		// dprintf(2, "entered 1 command not found exe\n");
		ft_data_destructor(data, 1, "command not found.\n");
	}
	if (access(data->command[start], X_OK) != 0 && ft_bin_path(data, ev, start) == 0)
	{
		// dprintf(2, "entered 2 command not found exe\n");
		ft_data_destructor(data, 1, "command not found.\n");
	}
	track = 0;
	while (data->command[start + track] != NULL && ft_strcmp(data->command[start + track], "<") == 'n' && ft_strcmp(data->command[start + track], "<<") == 'n' && ft_strcmp(data->command[start + track], ">") == 'n' && ft_strcmp(data->command[start + track], ">>") == 'n' && ft_strcmp(data->command[start + track], "|") == 'n')
		track++;
	// dprintf(2, "malloc here\n");
	output = malloc(sizeof(char *) * (track + 1));
	if (output == NULL)
	{
		// dprintf(2, "entered malloc not found exe\n");
		ft_data_destructor(data, -1, NULL);
	}
	track = 0;
	// dprintf(2, "here current = :%s:\n", data->command[start]);
	while (data->command[start] != NULL && ft_strcmp(data->command[start], "<") == 'n' && ft_strcmp(data->command[start], "<<") == 'n' && ft_strcmp(data->command[start], ">") == 'n' && ft_strcmp(data->command[start], ">>") == 'n' && ft_strcmp(data->command[start], "|") == 'n')
	{
		output[track++] = data->command[start++];
		// dprintf(2, "track = %d, output = :%s:\n", track - 1, output[track - 1]);
	}
	output[track] = NULL;
	execve(output[0], output, ev);
	free(output);
	ft_data_destructor(data, -1, NULL);
}

void	ft_piping(t_data *data, char *ev[], int track)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		ft_data_destructor(data, -1, NULL);
	pid = fork();
	if (pid < 0)
		ft_data_destructor(data, -1, NULL);
	if (pid > 0)
	{
		close(fd[1]);
		if (dup2(fd[0], 0) < 0)
			ft_data_destructor(data, -1, NULL);
	}
	else
	{
		close(fd[0]);
		if (dup2(fd[1], 1) < 0)
			ft_data_destructor(data, -1, NULL);
		ft_exe(data, ev, track);
	}
}

int	ft_execute(t_data *data, char *ev[])
{
	int		track;
	int		status;
	pid_t	pid;

	// dprintf(2, "entering execute\n");
	track = -1;
	while (++track < data->is_pipe)
		ft_piping(data, ev, track);
	// dprintf(2, "piping done\n");
	pid = fork();
	if (pid < 0)
		ft_data_destructor(data, -1, NULL);
	if (pid > 0)
	{
		// dprintf(2, "waiting..\n");
		while (wait(&status) > 0);
		// dprintf(2, "done waiting..\n");
	}
	else
	{
		if (data->is_pipe == 0)
			ft_exe(data, ev, 0);
		else
			ft_exe(data, ev, track);
	}
	return (1);
}
