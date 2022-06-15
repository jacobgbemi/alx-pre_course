#include "shell.h"

/**
 * main - main entry point of the program
 * Return: 0 if successful
*/

int main(void)
{
	char *buffer, **commands;
	list_t *linkedlist_path;
	int characters;
	size_t bufsize = BUFSIZE;

	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	linkedlist_path = path_list();
	while (1)
	{
		write(STDOUT_FILENO, "~$ ", 3);
		characters = getline(&buffer, &bufsize, stdin);
		if (characters == -1)
			perror("Error in reading line");

		commands = split_line(buffer);
		if (!commands)
			break;

		if (_builtin(commands[0]))
			_builtin(commands[0])(commands, linkedlist_path, buffer);
		else
			execute(commands, linkedlist_path);

		free(commands);

		if (characters == EOF)
		{
			free(buffer);
			continue;
		}

	}
	return (0);
}
