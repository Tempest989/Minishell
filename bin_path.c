#include "minishell.h"

int	ft_is_bin(char *ev, t_data *data, int position, int track)
{
	int		length;
	char	*check;

	while (data->command[position][track] != '\0')
		track++;
	length = 0;
	while (ev[length] != '\0' && ev[length] != ':')
		length++;
	check = malloc(sizeof(char) * (length + track + 2));
	if (check == NULL)
		ft_data_destructor(data, -1, NULL);
	length = -1;
	while (ev[++length] != '\0' && ev[length] != ':')
		check[length] = ev[length];
	check[length++] = '/';
	track = -1;
	while (data->command[position][++track] != '\0')
		check[track + length] = data->command[position][track];
	check[track + length] = '\0';
	if (access(check, X_OK) == 0)
	{
		free(data->command[position]);
		data->command[position] = check;
		return (1);
	}
	free(check);
	return (-1);
}

int	ft_bin_path(t_data *data, char *ev[], int position)
{
	int		length;
	int		path;

	length = 0;
	while (ev[length] != NULL && (ev[length][0] != 'P' || ev[length][1] != 'A'
		|| ev[length][2] != 'T' || ev[length][3] != 'H' || ev[length][4] != '='))
		length++;
	if (ev[length] == NULL)
		return (0);
	path = 5;
	while (ev[length][path] != '\0')
	{
		if (ft_is_bin(&ev[length][path], data, position, 0) == 1)
			return (1);
		while (ev[length][path] != '\0' && ev[length][path] != ':')
			path++;
		if (ev[length][path] == ':')
			path++;
	}
	return (0);
}

// int main(int ac, char **av, char **ev)
// {
// 	t_data data;

// 	data.command = malloc(sizeof(char *));
// 	if (data.command == NULL)
// 	{
// 		printf("first malloc erorr\n");
// 		return (0);
// 	}
// 	char *check = "hello";
// 	int track = 0;
// 	while (check[track] != '\0')
// 		track++;
// 	data.command[0] = malloc(sizeof(char) * (track + 1));
// 	if (data.command[0] == NULL)
// 	{
// 		free(data.command);
// 		printf("second malloc erorr\n");
// 		return (0);
// 	}
// 	track = 0;
// 	while (check[track] != '\0')
// 	{
// 		data.command[0][track] = check[track];
// 		track++;
// 	}
// 	data.command[0][track] = '\0';
// 	printf("before bin path\n");
// 	printf("bin output: %d\n", ft_bin_path(&data, ev, 0));
// 	printf("command = %s\n", data.command[0]);
// 	free(data.command[0]);
// 	free(data.command);
// 	return (0);
// }
