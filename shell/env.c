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
		/*free(abs_path);*/
		ep = ep->next;
	}
	return (NULL);
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
	pid_t child_pid;
	char *abs_path;
	int status;

	abs_path = search_os(argv[0], linkedlist_path);
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
			perror("execution failed\n");
        }
        else
	{
		wait(&status);
        }

}

char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return (dest);
}

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
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += 1024;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        perror("allocation error");
        exit(1);
      }
    }

    token = strtok(NULL, delimeter);
  }
  tokens[position] = NULL;
  return (tokens);
}

/**
  * free_list - frees a list
  * @head: beginning of the list
  */
void free_list(list_t *head)
{
	if (head == NULL)
		return;
	free_list(head->next);
	free(head->str);
	free(head);
}

int main(void)
{
       /* int status;*/
        char *buffer;
	list_t *linkedlist_path;
        int characters;
        char **commands;
        /*pid_t child_pid;*/
        size_t bufsize = 0;

        buffer = (char *)malloc(bufsize * sizeof(char));

        if( buffer == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }

	linkedlist_path = list_from_path();

        while (1)
        {
		write(STDOUT_FILENO, "~$ ", 3);
            	characters = getline(&buffer,&bufsize,stdin);

            	commands = split_line(buffer);
		
              	while (commands)
		{
			executor(commands, linkedlist_path);
			/*free(commands);*/
			/*free_list(linkedlist_path);*/
			if (commands == NULL)
				break;
		}
		
		if (characters == EOF)
		{
                	free(buffer);
                	continue;
            	}

        }

return (0);      
}
