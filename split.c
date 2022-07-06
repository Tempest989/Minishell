#include "minishell.h"

int	ft_find(char *input, int start, char locate)
{
	while (input[start])
	{
		if (input[start] == locate)
			return (start);
		start++;
	}
	return (-1);
}

int	ft_count(char *input)
{
	int	track;
	int	words;
	int	temp;

	track = 0;
	words = 1;
	while (input[track])
	{
		while (input[track] && input[track] == ' ')
			track++;
		if (input[track])
			words++;
		while (input[track] && input[track] != ' ')
		{
			if (input[track] == '\'' || input[track] == '"')
			{
				temp = ft_find(input, track + 1, input[track]);
				if (temp != -1)
					track = temp;
			}
			track++;
		}
	}
	return (words);
}

int	ft_assign(t_data *data, char *input)
{
	int	track;
	int	words;
	int	temp;
	int	last;

	track = 0;
	words = -1;
	while (input[track])
	{
		while (input[track] && input[track] == ' ')
			track++;
		if (input[track])
			words++;
		last = track;
		while (input[track] && input[track] != ' ')
		{
			if (input[track] == '\'' || input[track] == '"')
			{
				temp = ft_find(input, track + 1, input[track]);
				if (temp != -1)
					track = temp;
			}
			track++;
		}
		if (track - last > 0)
		{
			data->command[words] = malloc(sizeof(char) * (track - last + 1));
			if (data->command[words] == NULL)
			{
				printf("malloc error\n");
				return (-1);
			}
			temp = last;
			while (temp < track)
			{
				data->command[words][temp - last] = input[temp];
				temp++;
			}
			data->command[words][temp - last] = '\0';
		}
	}
	data->command[words + 1] = NULL;
	return (words + 1);
}

int	ft_split(t_data *data, char *input)
{
	data->command = malloc(sizeof(char) * ft_count(input));
	if (data->command == NULL)
	{
		printf("malloc error\n");
		return (-1);
	}
	return (ft_assign(data, input));
}

// int main()
// {
// 	t_data data;
// 	int num = ft_split(&data, "Hello World");
// 	for (int i = 0; i <= num; i++)
// 	{
// 		printf("%d: :%s:\n", i, data.command[i]);
// 		free(data.command[i]);
// 	}
// 	free(data.command);
// 	return (0);
// }