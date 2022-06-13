#include "shell.h"

/**
  * is_builtin - checks if cmd is a builtin
  * @cmd: command to find
  * Return: On success - pointer to function, On Failure - NULL pointer
 (* other useful shell builtins:
 (* pwd, echo, pushd, popd, type
 (* * requires ^Z
 (* fg, bg
 (*  * Requires ^R
 (* reverse-i-search **HISTORY**
*/
int (*is_builtin(char *cmd))()
{
	unsigned int i;
	builtin_cmds_t builds[] = {
		{"alias", _alias},
		{"cd", _cd},
		{"env", _env},
		{"exit", _exit_with_grace},
		{"history", _history},
		{"setenv", _setenv_usr},
		{NULL, NULL}
	};

	i = 0;
	while (*builds[i].fun != NULL)
	{
		if (strncmp(builds[i].cmd_str, cmd, _strlen(builds[i].cmd_str)) == 0)
			return (builds[i].fun);
		i++;
	}
	return (NULL);
}
/**
  * _exit_with_grace - Frees any remaining malloc'd spaces, and exits
  * @linkedlist_path: Linked list to free.
  * @buffer: buffer to free
  * @tokens: Check for other inputs
 (* * CHANGE TO VARIADIC LIST.
  * Return: -1 if exit fails.
  */
int _exit_with_grace(char **tokens, list_t *linkedlist_path, char *buffer)
{
	unsigned char exit_status;
	int i;

	for (i = 0; tokens[1] && tokens[1][i]; i++)
	{
		if (!_isnumber(tokens[1][i]))
		{
			_puts("numeric argument required, exiting\n");
			break;
		}
	}
	exit_status = tokens[1] && i >= _strlen(tokens[1]) ? atoi(tokens[1]) : 0;
	if (linkedlist_path && buffer && tokens)
	{
		free_list(linkedlist_path);
		linkedlist_path = NULL;
		free(buffer);
		buffer = NULL;
		free(tokens);
		tokens = NULL;
	}
	exit(exit_status);
	return (-1);
}
/**
  * _env - prints out the current environment
  * @tokens: tokenized strings
  * @environment: linked list environment
  * Return: 0 on success, -1 on catastrophic failure
  */
int _env(char **tokens, list_t *environment)
{
	char **envir;

	if (tokens[1])
		_puts("No arguments are necessary\n");
	envir = environ;
	if (!envir || !environ)
		return (-1);
	for ( ; *envir; envir++)
	{
		write(STDOUT_FILENO, *envir, _strlen(*envir));
		write(STDOUT_FILENO, "\n", 1);
	}
	environment++;
	return (0);
}

/**
 * _strlen - Returns the length of a string.
 * @s: string to evaluate.
 *
 * Return: The length of @i.
 */

int _strlen(char *s)
{
	int len = 0;

	while (s[len] != '\0')
		len++;
	return (len);
}

/**
 * _isdigit - checks if chars are digits
 * @c: char to check
 * Return: 1 if yes, 0 if no
 */
int _isnumber(int c)
{
	if(c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */

int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 * _puts - Prints a string to stdout.
 * @str: The string to be printed.
 */

void _puts(char *str)
{
	while (*str)
		_putchar(*str++);

	_putchar('\n');
}

/**
  * _cd - changes working directory
  * @tokens: argument list
  * Return: 0 on success
  */
int _cd(char **tokens)
{
	char *target;
	char pwd[BUFSIZE];
	char *home;

	home = getenv("HOME");
	if (tokens[1])
	{
		if (tokens[1][0] == '~' && !tokens[1][1])
			target = home;
		else if (tokens[1][0] == '-' && !tokens[1][1])
			target = getenv("OLDPWD");
		else
			target = tokens[1];
	}
	else
		target = home;
	if (target == home)
		chdir(target);
	else if (access(target, F_OK | R_OK) == 0)
		chdir(target);
	else
		puts("Could not find directory\n");
	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", getcwd(pwd, sizeof(pwd)), 1);
	return (0);
}



/**
  * _setenv_usr - sets environmental variables as user defines
  * @tokens: KEY=VALUE pair
  * Return: 0 on success, -1 on failure
  */
int _setenv_usr(char **tokens)
{
	int i, status, wc;
	char *key, *value;

	for (i = 0, wc = 1; tokens[1][i]; i++)
		if (tokens[1][i] == '=')
			wc++;
	for (i = 0; tokens[i]; i++)
		;
	if (!tokens[1] || i == 0 || wc != 2)
	{
		_puts("setenv: Usage: setenv KEY=VALUE\n");
		return (-1);
	}
	key = strtok(tokens[1], "=");
	value = strtok(NULL, "=");
	status = setenv(key, value, 0);
	if (status == 0)
		return (status);
	return (-1);
}
/**
  * _alias - sets aliases or prints them out when no options are supplied
  * Return: 0
  */
int _alias(void)
{
	_puts("alias: usage: alias [-p] [name[=value] ... ]\n");
	_puts("\tSet or view aliases.\n\tSet with name=value\n");
	_puts("\tView list of aliases with no arugments or -p\n");
	return (0);
}
/**
  * _history - prints out history with no options,
  *  or clears history with -c
  * Return: 0 on success, 1 if history cannot be cleared.
  */
int _history(void)
{
	_puts("history: usage: history [-c]\n");
	_puts("\tView the history of commands\n ");
	_puts("\t'history -c' clears the history\n");
	return (0);
}




