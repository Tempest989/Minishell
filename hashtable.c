#include "minishell.h"

char	*ft_search_table(t_data *data, char *key, int *error)
{
	t_list	*temp;
	int		track;
	char	*output;

	if (key == NULL)
		return (NULL);
	// printf("\n\nsearching using key: %s\n\n", key);
	temp = data->table[key[0] % 128];
	while (temp != NULL)
	{
		if (ft_strcmp(temp->key, key) == 'y')
		{
			track = 0;
			while (temp->value[track])
				track++;
			output = malloc(sizeof(char) * (track + 1));
			if (output == NULL)
			{
				*error = -1;
				return (NULL);
			}
			track = -1;
			while (temp->value[++track])
				output[track] = temp->value[track];
			output[track] = '\0';
			return (output);
		}
		temp = temp->next;
	}
	return (NULL);
}

int	ft_add_table(t_data *data, char *key, char *value)
{
	t_list	*temp1;
	int		track;

	temp1 = data->table[key[0] % 128];
	printf("entering loop\n");
	while (temp1 != NULL)
	{
		printf("here\n");
		if (ft_strcmp(temp1->key, key) == 'y')
		{
			// printf("entered\n");
			free(temp1->value);
			track = 0;
			while (value[track])
				track++;
			temp1->value = malloc(sizeof(char) * (track + 1));
			if (temp1->value == NULL)
				return (-2);
			track = -1;
			while (value[++track])
				temp1->value[track] = value[track];
			temp1->value[track] = '\0';
			return (1);
		}
		// printf("passed if\n");
		temp1 = temp1->next;
	}
	printf("after\n");
	t_list *temp = (t_list *)malloc(sizeof(t_list) * 1);
	printf("after malloc for temp\n");
	if (temp == NULL)
		return (-2);
	track = 0;
	while (value[track])
		track++;
	temp->value = malloc(sizeof(char) * (track + 1));
	if (temp->value == NULL)
		return (-2);
	track = 0;
	while (key[track])
		track++;
	temp->key = malloc(sizeof(char) * (track + 1));
	if (temp->key == NULL)
		return (-2);
	track = -1;
	while (value[++track])
		temp->value[track] = value[track];
	temp->value[track] = '\0';
	track = -1;
	while (key[++track])
		temp->key[track] = key[track];
	temp->key[track] = '\0';
	if (data->table[key[0] % 128] == NULL)
	{
		// printf("null checked\n");
		temp->next = NULL;
	}
	else
	{
		// printf("not NULL!!\n");
		temp->next = data->table[key[0] % 128];
	}
	data->table[key[0] % 128] = temp;
	return (1);
}

// void	ft_data_destructor(t_data *data, int flag, char *message)
// {
// 	int	track;

// 	if (data->pipe_locale != NULL)
// 		free(data->pipe_locale);
// 	for (int i = 0; i < 128; i++)
// 	{
// 		while (data->table[i] != NULL)
// 		{
// 			t_list *temp = data->table[i]->next;
// 			free(data->table[i]->key);
// 			free(data->table[i]->value);
// 			temp = data->table[i]->next;
// 			free(data->table[i]);
// 			data->table[i] = temp;
// 		}
// 	}
// 	free(data->table);
// 	track = 0;
// 	while (data->command != NULL && data->command[track] != NULL)
// 		free(data->command[track++]);
// 	if (data->command != NULL)
// 		free(data->command);
// 	// if (message != NULL)
// 	// {
// 	// 	track = 0;
// 	// 	while (message[track++] != '\0');
// 	// 	write(2, message, track);
// 	// }
// 	// if (flag == -2)
// 	// 	return ;
// 	// else if (flag == -1)
// 	// {
// 	// 	perror("Minishell: ");
// 	// 	exit(errno);
// 	// }
// 	// else
// 	// 	exit(flag);
// }

// void	ft_initialize(t_data *data)
// {
// 	data->is_pipe = 0;
// 	data->pipe_locale = NULL;
// 	data->command_num = 0;
// 	data->command = NULL;
// 	data->in_out_fd[0] = -1;
// 	data->in_out_fd[1] = -1;
// 	data->in_out_fd[2] = -1;
// 	data->in_out_fd[3] = -1;
// 	data->in_out_fd[4] = -1;
// 	data->in_out_fd[5] = -1;
// 	data->table = malloc(sizeof(t_list *) * (128));
// 	for (int i = 0; i < 128; i++)
// 		data->table[i] = NULL;
// }

// char	ft_strcmp(const char *str1, const char *str2)
// {
// 	int	track;

// 	track = 0;
// 	while (str1[track] && str2[track] && str1[track] == str2[track])
// 		track++;
// 	if (str1[track] == '\0' && str2[track] == '\0')
// 		return ('y');
// 	return ('n');
// }

// // $apple $app=89 NULL

// int main()
// {
// 	t_data test;
// 	ft_initialize(&test);
// 	printf("starting\n");
// 	ft_add_table(&test, "apple", "01");
// 	printf("onto second\n");
// 	ft_add_table(&test, "app", "02");
// 	test.command_num = 1;
// 	test.command = malloc(sizeof(char *) * 2);
// 	test.command[1] = NULL;
// 	test.command[0] = malloc(sizeof(char) * 8);
// 	// test.command[1] = malloc(sizeof(char) * 8);
// 	char *output = "$app=89";
// 	for (int i = 0; i < 7; i++)
// 		test.command[0][i] = output[i];
// 	test.command[0][7] = '\0';
// 	// output = "$app=89";
// 	// for (int i = 0; i < 7; i++)
// 	// 	test.command[0][i] = output[i];
// 	// test.command[0][7] = '\0';
// 	int check = ft_environ_checks(&test, NULL);
// 	printf("\n\ncheck = %d\n", check);
// 	for (int i = 0; i < 1; i++)
// 	{
// 		if (test.command[i] != NULL)
// 			printf("%s\n", test.command[i]);
// 		else
// 			printf("is NULL\n");
// 	}
// 	printf("\n");
// 	// printf("done:\n");
// 	t_list *temp = test.table['a' % 128];
// 	for (int i = 0; i < 2; i++)
// 	{
// 		printf("%s :: %s\n", temp->key, temp->value);
// 		if (temp->next == NULL)
// 		{
// 			printf("next = NULL\n");
// 			break;
// 		}
// 		temp = temp->next;
// 	}
// 	int error = 0;
// 	output = ft_search_table(&test, NULL, &error);
// 	if (output == NULL)
// 		printf("output has returned NULL:: error: %d\n", error);
// 	else
// 	{
// 		printf("%s\n", output);
// 		free(output);
// 	}
// 	// printf("%s :: %s\n", temp->key, temp->value);
// 	ft_data_destructor(&test, -2, NULL);
// 	return (0);
// }