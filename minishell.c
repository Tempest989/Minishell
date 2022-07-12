#include "minishell.h"

// void	ft_data_destructor(t_data data)
// {

// }

// void	ft_execute(t_data data)
// {

// }

int	main()
{
	t_data	data;
	int		result;
	
	while (1)
	{
		data.is_pipe = -1;
		data.command_num = 0;
		data.in_out_fd[0] = -1;
		data.in_out_fd[1] = -1;
		data.in_out_fd[2] = -1;
		data.in_out_fd[3] = -1;
		result = ft_reading(&data);
		// if (result > 0)
		// 	result = ft_execute(&data);
		if (result == -1)
			break ;
		if (data.in_out_fd[0] != -1)
		{
			dprintf(2, "closing input file descriptor\n");
			if (close(data.in_out_fd[0]))
			{
				perror("close");
			}
			dup2(data.in_out_fd[4], 0);
		}
		if (data.in_out_fd[1] != -1)
		{
			dprintf(2, "closing output file descriptor\n");
			// close(data.in_out_fd[1]);
			if (close(data.in_out_fd[1]))
			{
				perror("close");
			}
			dup2(data.in_out_fd[5], 1);
		}
		if (data.in_out_fd[0] != -1 && ft_strcmp(data.command[data.in_out_fd[2]], "<<") == 'y')
		{
			dprintf(2, "unlink calling\n");
			if (unlink(".temp"))
			{
				perror("unlink");
			}
		}
		dprintf(2, "in = %d out = %d\n", data.in_out_fd[0], data.in_out_fd[1]);
		for (int i = 0; i <= data.command_num; i++)
			dprintf(2, "%d: :%s:\n", i, data.command[i]);
		for (int i = 0; i <= data.command_num; i++)
		{
			dprintf(2, "%d: :%s:\n", i, data.command[i]);
			free(data.command[i]);
		}
		free(data.command);
	}
	// ft_data_destructor(data);
	rl_clear_history();
	return (0);
}
