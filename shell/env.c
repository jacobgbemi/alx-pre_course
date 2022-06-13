#include "shell.h"


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
	{
		/*free(dupstr);*/
		return (NULL);
	}
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
	/*free(dupstr);*/
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
	char buffer[1024];
	list_t *dir;

	dir = NULL;
	/*buffer = malloc(sizeof(char) * BUFSIZE);*/
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
		/*free(buffer);*/
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
		/*free(abs_path);*/
	}
	while (ep != NULL)
	{
		abs_path = strdup(ep->str);
		if (abs_path == NULL)
		{
			/*free(abs_path);*/
			return (NULL);
		}
		/*free(abs_path);*/
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
 * c_exit - frees user's typed command and linked list before exiting
 * @str: user's typed command
 * @env: input the linked list of envirnment
 */
void __exit(char **str, list_t *env)
{
	free_double_ptr(str);
	free_list(env);
	_exit(0);
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
		/*free(abs_path);*/
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
		{
			perror("execution failed\n");
			__exit(argv, linkedlist_path);
		}
		/*free(abs_path);*/
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
  /*free(s);*/
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
    /*free(tokens);*/
    /*free_double_ptr(tokens);*/
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
	list_t *holder;
	while (head != NULL)
	{
		holder = head;
		head = head->next;
		free(holder->str);
		free(holder);
	}
}

/**
 * free_double_ptr - free malloced arrays
 * @str: array of strings
 */
void free_double_ptr(char **str)
{
	int i = 0;

	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}


int main(void)
{
       /* int status;*/
        char *buffer;
	list_t *linkedlist_path;
        int characters;
        char **commands;
	size_t bufsize = BUFSIZE;
        /*pid_t child_pid;*/
        
        buffer = (char *)malloc(bufsize * sizeof(char));

        if( buffer == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }

	linkedlist_path = list_from_path();

        do {
		write(STDOUT_FILENO, "~$ ", 3);
            	characters = getline(&buffer,&bufsize,stdin);

            	commands = split_line(buffer);
		if (!commands)
			break;
			
		if (is_builtin(commands[0]))
			is_builtin(commands[0])(commands, linkedlist_path, buffer);
		else
			executor(commands, linkedlist_path);
		
		free(commands);
				
/*              	while (commands)
*		{
*			if (!commands)
*				break;
*			executor(commands, linkedlist_path);
*			*commands = strtok(NULL, "\n");
*			free(commands);
*			free_list(linkedlist_path);
*		}
*/				
		if (characters == EOF)
		{
                	free(buffer);
                	continue;
            	}

        } while (1);

return (0);      
}
