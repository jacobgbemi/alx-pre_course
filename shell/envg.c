#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


char **_environ(char **env)
{

	while (env)
    	{
        	return(env++);
    	}
    	return (0);
}

char *_getenv(char *name)
{
	int index, len;
	char **environ;
	
	environ = _environ(&name);
	len = strlen(name);
	for (index = 0; environ[index]; index++)
	{
		if (strncmp(name, environ[index], len) == 0)
			return (environ[index]);
	}

	return (NULL);

}


int main () {
   printf("PATH : %s\n", _getenv("PATH"));
   printf("HOME : %s\n", _getenv("HOME"));
   printf("ROOT : %s\n", _getenv("ROOT"));

   return(0);
}
