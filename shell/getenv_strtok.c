#include "shell.h"


char *_getenv(const char *name)
{
	int index, len;
	extern char **environ;

	len = strlen(name);
	for (index = 0; environ[index]; index++)
	{
		if (strncmp(name, environ[index], len) == 0)
			return (environ[index]);
	}

	return (NULL);

}

char *_strtok(char *env)
{
	char *buffer, *dir, *token;
	int bufsize = 1024;

	buffer = malloc(sizeof(char) * bufsize);
	dir = _getenv(env);
	strcpy(buffer, dir);

	token = strtok(buffer, ":");
		while (token)
		{
			printf("buffer strtok: %s\n", token);
			token = strtok(NULL, ":");
		}


}

