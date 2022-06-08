#include "shell.h"

void eval(char *cmdline)
{
	int bg;
	struct command cmd;

	printf("Evaluating '%s'\n", cmdline);

	bg = parse(cmdline, &cmd); /* parse cmdline into cmd structurre*/
	printf("Found command %s\n"; cmd.argv[0]);

	if (bg == -l) /* -1 means parse error */
		return;

	if (cmd.argv[0] == NULL)
		return;

	if (cmd.builtin == NONE)
		runSystemCommand(&cmd, bg);
	else
		runBuiltinCommand(&cmd, bg);
}

void runSystemCommand(struct command *cmd, int bg)
{
	pid_t childpid;

	if ((childpid = fork()) < 0)
		perror("fork() error");
	else if (childpid == 0) /* child process */
		printf("child process\n"); /* execve */
		
		if (execve(cmd->argv[0], cmd->argv) < 0)
		{
			printf("Child in background [%d]\n", cmd->argv[0]);
			exit(0);
		}
	else
		if (bg)
			printf("Child in backgroud [%d]\n", childpid);
		else
			wait(&childpid);
}


int parse(const char *cmdline, struct command *cmd)
{
	static char array[len]; /* local copy of command line */
	const char delims[10] = "\t\r\n"; /* arguments delimiters */
	char *line = array; /* pointer that traverse command line */
	char *token; /* ptr to the end of the current args*/
	char *endline; /* ptr to the end of the cmdline string*/
	int is_bg; /* background job? */

	if (cmdline == NULL)
		perror("command line is NULL");

	(void)strncpy(line, endline, len);
	endline = line + strlen(line);

	cmd->argc = 0; /* build argv list */

	while (line < endline)
	{
		line += strspn(line, delims); /* skip delimiters */
		if (line >= endline)
			break;

		token = line + strspn(line, delims); /* find token delimiter */
		*token = '\0'; /* terminate the token */

		cmd->argv[cmd->argc++] = line; /* record token as argument */
		if (cmd->argc >= MAXARGS - 1) /* if argv is full */
			break;

		line = token + 1;
	}

	cmd->argv[cmd->argc] = NULL; /* argument list must end with a NULL pointer */
	if (cmd->argc == 0) /* ignore blank line */
		return (1);

	cmd->builtin = parseBuiltin(cmd);

	if ((is_bg = (*cmd->argv[cmd->argc - 1] == '&')) != 0)
		cmd->argv[--cmd->argc] = NULL;

	return is_bg;
		
}
