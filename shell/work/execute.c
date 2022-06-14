#include "shell.h"

char **split_line(char *line)
{
  int bufsize = 1024, position = 0;
  char *delimeter =  " \t\r\n\a";
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
  char *s = malloc(bufsize * sizeof(char));

  _strcpy(s,line);

  if (!tokens) {
    perror("allocation error");
    exit(1);
  }

  token = strtok(s, delimeter);
  /*free(s);*/
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += 1024;
      tokens = _realloc(*tokens, token[position], bufsize * sizeof(char*));
      if (!tokens) {
        perror("allocation error");
        exit(1);
      }
    }

    token = strtok(NULL, delimeter);
    /*free(tokens);*/
    /*free_double_ptr(tokens);*/
  }
  tokens[position] = NULL;
  return (tokens);
}


/**
  * executor - executes a command
 (* by searching through PATH
  * @argv: array of tokens, ie. argument vectors
  * @linkedlist_path: PATH in LL form
  * Return: 0 on success, -1 on failure
  */
void execute(char *argv[], list_t *linkedlist_path)
{
	pid_t child_pid;
	char *abs_path;
	int status;

	abs_path = _which(argv[0], linkedlist_path);
	if (!abs_path)
	{
		perror("command not found\n");
		return;
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
                exit (1);
        }
        if (child_pid == 0)
        {
		if (execve(abs_path, argv, environ) == -1)
		{
			perror("execution failed\n");
			__exit(argv, linkedlist_path);
		}
		
        }
        else
	{
		wait(&status);
		
        }

}

