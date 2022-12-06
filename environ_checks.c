#include "minishell.h"

int	ft_$_find(char *input, int start, char locate, int temp)
{
	// printf("\nlocate = %c, input starting char = %c\n", locate, input[start]);
	// printf("input = %s :: %d\n\n", input, start);
	while (input[start])
	{
		if (input[start] == '\'')
		{
			temp = start;
			while (input[++start] && input[start] != '\'');
			if (!input[start])
				start = temp;
		}
		if (input[start] == locate)
			return (start);
		start++;
	}
	if (locate == '\0')
		return (start);
	return (-1);
}

char	*ft_environ_scan(char *input, char *output, t_data *data)
{
	int start = 0;
	while (input[start])
		start++;
	// printf("before output malloc %d\n", start);
	output = malloc(sizeof(char) * (start + 1));
	// printf("after output malloc\n");
	if (output == NULL)
		return (NULL);
	start = -1;
	while (input[++start])
		output[start] = input[start];
	output[start] = '\0';
	start = 0;
	// printf("output = %s\n", output);
	while (1)
	{
		// printf("length check\n");
		int length = ft_$_find(output, start, '$', 0);
		// printf("length = %d\n", length);
		if (length < 0)
			break;
		// printf("pass length check\n");
		int track = ft_find(output, 0, '\0');
		int key = ft_find(output, length, ' ');
		if (key == -1)
			key = track - length;
		char *new = malloc(sizeof(char) * (key + 1));
		if (new == NULL)
			return (NULL);
		// printf("key num = %d\n", key);
		int temp = 0;
		while (output[length + temp + 1] && output[length + temp + 1] != ' ')
		{
			new[temp] = output[length + temp + 1];
			temp++;
		}
		new[temp] = '\0';
		int error = 0;
		// printf("new = %s\n", new);
		char *value = ft_search_table(data, new, &error);
		free(new);
		if (value == NULL && error == -1)
			return (NULL);
		else if (value == NULL)
		{
			// printf("hit the continue\n");
			start = length + 1;
			continue;
		}
		int add = ft_find(value, 0, '\0');
		char *next = malloc(sizeof(char) * (track - (key - length) + add + 1));
		if (next == NULL)
			return (NULL);
		temp = -1;
		while (output[++temp] != '$')
			next[temp] = output[temp];
		error = -1;
		while (value[++error])
			next[temp + error] = value[error];
		temp += error;
		while (output[++key + error])
			next[error++] = output[key];
		next[error] = '\0';
		free(output);
		free(value);
		output = next;
	}
	// printf("returning from environ scan\n");
	return (output);
}

int	ft_environ_checks(t_data *data, char *check)
{
	for (int i = 0; i < data->command_num; i++)
	{
		int temp1 = ft_$_find(data->command[i], 1, '=', 0);
		int temp2 = ft_find(data->command[i], 1, ' ');
		if (data->command[i][0] == '$' && (temp1 < temp2 || (temp1 > 0 && temp2 == -1))) 
		{
			// printf("inside if statement =\n");
			int track = 1;
			while (data->command[i][track] != '=')
				track++;
			// printf("before malloc key\n");
			char *key = malloc(sizeof(char) * track);
			if (key == NULL)
				return (-2);
			track = 0;
			while (data->command[i][++track] != '=')
				key[track - 1] = data->command[i][track];
			key[track - 1] = '\0';
			// printf("key = %s\n", key);
			char *value = ft_environ_scan(&data->command[i][track + 1], NULL, data);
			if (value == NULL)
			{
				free(key);
				return (-2);
			}
			// printf("value = %s\nbefore add table\n", value);
			int output = ft_add_table(data, key, value);
			// printf("after add table\n");
			free(value);
			free(key);
			// printf("free-ing key\n");
			// printf("\noutput = %d\n", output);
			if (output < 0)
				return (output);
			// printf("\n\ncommands currently:\n");
			// for(int k = 0; k < data->command_num; k++)
			// 	printf("%s\n", data->command[k]);
			free(data->command[i]);
			data->command[i] = NULL;
			// printf("before return 2 for command_num == 1\n");
			if (data->command_num == 1)
				return (2);
			output = i;
			while (output <= data->command_num)
			{
				data->command[output] = data->command[output + 1];
				output++;
			}
			data->command_num--;
			// free(data->command[output + 1]);
			// data->command[output] = NULL;
			// printf("\n\ncommands currently:\n");
			// for(int k = 0; k < data->command_num; k++)
			// 	printf("%s\n", data->command[k]);
			i--;
		}
		else if (ft_$_find(data->command[i], 0, '$', 0) >= 0)
		{
			// printf("here\n");
			check = ft_environ_scan(data->command[i], NULL, data);
			// printf("check == NULL.....\n");
			if (check == NULL)
				return (-2);
			// printf("check = %s\n", check);
			free(data->command[i]);
			data->command[i] = check;
		}
		// printf("\n\n\nnext command\n\n\n");
	}
	return (0);
}