#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int len = 1024;
char prompt[] = "hsh> "; /* command line promt*/

int main(int argc, char **argv)
{
	char cmdline[len];/* buffer for fets*/
	
	while (1) /* repeat forever */
	{
		printf("%s", prompt); 
		
		if (fgets(cmdline, len, stdin) == NULL) && ferror(stdin)
			perror("fgets error");
		
		if (feof(stdin))
		{
			printf("\n");
			exit(0);
		}
		
		cmdline[strlen(cmdline) - 1] = '\0';/* remove trailing newline */
			
		eval(cmdline);/* evaluate command line*/
	}
	return (0);
}
