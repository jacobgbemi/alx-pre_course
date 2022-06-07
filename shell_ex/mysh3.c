#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAXARGS 128 /* maxargs in struct can't be variable, use define */

struct command{
	int argc; /* number of args */
	char *argv[MAXARGS]; /* arguments list */
	enum built_t{ /* is argv[0] a builtin command? */
		NONE, QUIT, JOBS, BG, FG
	} builtin;
};


void eval(char *cmdline)
{
	int bg;
	struct command cmd;

	printf("Evaluating '%s'\n", cmdline);

	bg = parse(cmdline, &cmd); /* parse cmdline into cmd structurre*/
	printf("Found command %s\n"; cmd.argv[0]);

	if (bg == -l) /* -1 means parse error */
		return;
}

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
