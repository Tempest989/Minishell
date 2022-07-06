#include "minishell.h"

// void	ft_data_destructor(t_data data)
// {

// }

// void	ft_execute(t_data data)
// {

// }

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
		printf("line empty\n");
		free(line);
		return (-1);
	}
	int num = ft_split(data, line);
	for (int i = 0; i <= num; i++)
	{
		printf("%d: :%s:\n", i, data->command[i]);
		free(data->command[i]);
	}
	free(data->command);
	// printf("%s", line);
	add_history(line);
	free(line);
	return (1);
}

int	main()
{
	t_data	data;
	int		result;
	while (1)
	{
		result = ft_reading(&data);
		// if (result == 1)
		// 	ft_execute(data);
		if (result == -1)
			break ;
	}
	// ft_data_destructor(data);
	rl_clear_history();
	return (0);
}
