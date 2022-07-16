.PHONY: all clean fclean re

NAME = minishell

all:
	gcc -Wall -Werror -Wextra minishell.c split.c reading.c double_left.c bin_path.c execute.c -lreadline -o $(NAME)

$(NAME): all

clean:
	@if [ -f "$(NAME)" ]; then\
		rm $(NAME);\
		printf "rm $(NAME)\n";\
	else\
		printf "$(NAME) Doesn't Exist so It Cannot Be Removed\n";\
	fi

fclean: clean

re: clean all
