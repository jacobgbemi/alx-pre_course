#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return (dest);
}

char **split_line(char *line)
{
  int bufsize = 1024, position = 0;
  char *delimeter =  " \t\r\n\a";
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
  char *s = malloc(bufsize * sizeof(char));
  char s[strlen(line) + 1];

  _strcpy(s,line);

  if (!tokens) {
    perror("allocation error");
    exit(1);
  }

  token = strtok(s, delimeter);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += 1024;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        perror("allocation error");
        exit(1);
      }
    }

    token = strtok(NULL, delimeter);
  }
  tokens[position] = NULL;
  return tokens;
}

int main(void)
{
        int status;
        char *buffer;
        int characters;
        char **commands;
        pid_t child_pid;
        size_t bufsize = 0;

        buffer = (char *)malloc(bufsize * sizeof(char));

        if( buffer == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }

        while (1)
        {
            write(STDOUT_FILENO, "~$ ", 3);
            characters = getline(&buffer,&bufsize,stdin);

            child_pid = fork();

            if (child_pid == -1)
            {
                perror("Error:");
                exit (1);
            }
            if (child_pid == 0)
            {
                commands = split_line(buffer);
                execve(commands[0], commands, NULL);
            }
            else {
                 wait(&status);
            }

            if (characters == EOF)
            {
                free(buffer);
                continue;
            }

        }

return (0);      
}
