#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 1024
extern char **environ;
/**
  * struct environ_path - linked list from PATH
  * @path: path in the format /usr/bin
  * @len: length of the string
  * @next: points to the next node
  */
typedef struct env_path
{
	char *str;
	unsigned int len;
	struct env_path *next;
} list_t;

/**
  * add_node - adds a new node to the end of a linked list
  * @head: head of the linked list
  * @str: string
  * @len: length of the string
  * Return: pointer to the current position in the list
  **/
list_t *add_node(list_t **head, char *str, unsigned int len)
{
	list_t *new, *holder;
	char *dupstr;

	if (str == NULL)
		return (NULL);
	dupstr = strdup(str);
	if (dupstr == NULL)
		return (NULL);
	new = malloc(sizeof(list_t));
	if (new == NULL)
		return (NULL);
	new->str = dupstr;
	new->len = len;
	new->next = NULL;

	if (*head == NULL)
	{
		*head = new;
		return (*head);
	}
	holder = *head;
	while (holder->next != NULL)
		holder = holder->next;
	holder->next = new;
	return (*head);
}
/**
  * list_from_path - builds a linked list from PATH
  * Return: pointer to linked list
  */
list_t *list_from_path(void)
{
	unsigned int len, i, j;
	char *env;
	char buffer[BUFSIZE];
	list_t *dir;

	dir = NULL;
	len = i = j = 0;
	env = getenv("PATH");
	while (*env)
	{
		buffer[j++] = *env;
		len++;
		if(*env == ':')
		{
			len--;
			buffer[j - 1] = '/';
			buffer[j] = '\0';
			add_node(&dir, buffer, len);
			len = j = 0;
		}
		env++;
	}
	return (dir);
}

/**
  * environ_linked_list - builds a linked list from PATH
  * Return: pointer to linked list
  */
list_t *environ_linked_list(void)
{
	int i, j;
	char **env;
	list_t *ep;

	ep = NULL;
	i = j = 0;
	env = environ;
	while (env[i])
	{
		add_node(&ep, env[i], (unsigned int)strlen(env[i]));
		i++;
	}
	return (ep);
}




char *search_os(char *cmd, list_t *linkedlist_path)
{
	int status;
	char *abs_path;
	list_t *ep;

	ep = linkedlist_path;
	if (ep == NULL || cmd == NULL)
		return (NULL);
	if ((strncmp(cmd, "/", 1) == 0
			|| strncmp(cmd, "./", 2) == 0)
			&& access(cmd, F_OK | X_OK) == 0)
	{
		abs_path = strdup(cmd);
		return (abs_path);
	}
	while (ep != NULL)
	{
		abs_path = strdup(ep->str);
		if (abs_path == NULL)
			return (NULL);
		abs_path = strcat(abs_path, cmd);
		if (abs_path == NULL)
			return (NULL);
		status = access(abs_path, F_OK | X_OK);
		if (status == 0)
			return (abs_path);
		free(abs_path);
		ep = ep->next;
	}
	return (NULL);
}

/**
  * parser - parses a string into tokens
  * @str: string to parse
  * @delimit: delimiters chosen by user
  * Return: Double pointer to array of tokens
  */
char **parser(char *str)
{
	char **tokens, *token;
	unsigned int i, wc;

	wc = strlen(str);
	tokens = malloc((wc + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc failed\n");
		return (NULL);
	}
	tokens[0] = token = strtok(str, " ");
	for (i = 1; token; i++)
		tokens[i] = token = strtok(NULL, " ");
	return (tokens);
}


/**
  * executor - executes a command
 (* by searching through PATH
  * @argv: array of tokens, ie. argument vectors
  * @linkedlist_path: PATH in LL form
  * Return: 0 on success, -1 on failure
  */
void executor(char *argv[], list_t *linkedlist_path)
{
	pid_t child_status = 0;
	char *abs_path;

	abs_path = search_os(argv[0], linkedlist_path);
	if (!abs_path)
	{
		perror("command not found\n");
		return;
	}

	switch (child_status = fork())
	{
	case -1:
		perror("fork failed\n");
		break;
	case 0:
		if (execve(abs_path, argv, environ) == -1)
			perror("execution failed\n");
		break;
	default:
		free(abs_path);
		if (wait(NULL) == -1)
			perror("wait failed\n");
		break;
	}
}



int main(void)
{
	char *buffer, *cmd, **tokens;
	size_t len = 1024;
	int read;
	/*pid_t child_pid;*/
	list_t *linkedlist_path;

	buffer = malloc(sizeof(size_t) * len);
	linkedlist_path = list_from_path();

	while (1)
	{
		printf("$ ");
		read = getline(&buffer, &len, stdin);
		/*if (read != EOF)
		*	buffer = argv[ac++];*/
		cmd = strtok(buffer, " ");
		while (cmd)
		{
			tokens = parser(cmd);
			executor(tokens, linkedlist_path);
/*			child_pid = fork();
*			if (child_pid == -1)
*				return (1);
*			if (child_pid == 0)
*			{
*				executor(cmd, linkedlist_path);
*			}
*			else
*			{
*				free(cmd);
*				wait(&status);
*			}
*			cmd = strtok(NULL, " ");
*/		}

		free(tokens);
		cmd = strtok(NULL, " ");
		if (read == EOF)
		{
			free(buffer);
			continue;
		}
		
	}

	return (0);


}
