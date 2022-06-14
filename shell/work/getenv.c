#include "shell.h"

char *_getenv(const char *name)
{
	int index, len;
	extern char **environ;

	len = _strlen(name);
	for (index = 0; environ[index]; index++)
	{
		if (strncmp(name, environ[index], len) == 0)
			return (environ[index]);
	}

	return (NULL);

}

