#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>



void type_prompt()
{
	char *buffer;
	size_t len;

	static int first_time = 1;
	if (first_time) /* clear sreen for the first time */
	{
		/*const char* CLEAR_SCREEN-ANSI = "\e[l; lH\e[2J";*/
		/*const char* clear_screen = NULL;
		*write(STDOUT_FILENO, clear_screen, 12);*/
		printf("$ ");
		getline(&buffer, &len, stdin);
		first_time = 0;

	}

	/*printf("#");  display prompt */
}

void read_command(char cmd[], char *par[])
{
	char *line;
	size_t len = 1024;
	int count = 0, i = 0, j = 0;
	char *array[100], *pch;

	line = malloc(sizeof(char) * len);

	for (;;) /* read one line */
	{
		int read = getline(&line, &len, stdin);
		if (read == EOF)
			break;
	}

	if (count == 1)
		return;
	pch = strtok(line, "\n"); /* break into tokens (words) */

	while (pch != NULL) /* parse the line into words */
	{
		array[i++] = strdup(pch);
		pch = strtok(NULL, "\n");
	} 

	strcpy(cmd, array[0]); /* first word is the command */
	for (j = 0; j < i; j++) /* other words are parameter */
		par[j] = array[j];
	par[i] = NULL; /* NULL - terminate the parameter list */
}


int main()
{
	char cmd[100], command[100], *parameters[20];
	char *envp[] = {(char *) "PATH=/bin", 0}; /* environment variable */
	while (1) /* repeat forever */
	{
		type_prompt(); /* display prompt on screen */
		read_command(command, parameters); /* read input from terminal */
		if (fork() != 0) /* parent */
			wait(NULL); /* wait for child */
		else
		{
			strcpy(cmd, "/bin/");
			strcat(cmd, command);
			execve(cmd, parameters, envp); /* execute command */
		}
		
		if (strcmp(command, "exit") == 0)
			break;
	}
	return (0);
}
