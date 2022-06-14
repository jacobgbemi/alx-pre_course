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
list_t *path_list(void)
{
	unsigned int len, i, j;
	char *env;
	char buffer[1024];
	list_t *dir;

	dir = NULL;
	/*buffer = malloc(sizeof(char) * BUFSIZE);*/
	len = i = j = 0;
	env = _getenv("PATH");
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
list_t *environ_list(void)
{
	int i, j;
	char **env;
	list_t *path;

	path = NULL;
	i = j = 0;
	env = environ;
	while (env[i])
	{
		add_node(&path, env[i], (unsigned int)_strlen(env[i]));
		i++;
	}
	return (path);
}




char *_which(char *cmd, list_t *linkedlist_path)
{
	int status;
	char *abs_path;
	list_t *path;

	path = linkedlist_path;
	if (path == NULL || cmd == NULL)
		return (NULL);
	if ((_strncmp(cmd, "/", 1) == 0
			|| _strncmp(cmd, "./", 2) == 0)
			&& access(cmd, F_OK | X_OK) == 0)
	{
		abs_path = _strdup(cmd);
		return (abs_path);
		/*free(abs_path);*/
	}
	while (path != NULL)
	{
		abs_path = strdup(path->str);
		if (abs_path == NULL)
		{
			/*free(abs_path);*/
			return (NULL);
		}
		/*free(abs_path);*/
		abs_path = _strcat(abs_path, cmd);
		if (abs_path == NULL)
			return (NULL);
		status = access(abs_path, F_OK | X_OK);
		if (status == 0)
			return (abs_path);
		free(abs_path);
		path = path->next;
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
void execute(char *argv[], list_t *linkedlist_path)
{
	pid_t child_pid;
	char *abs_path;
	int status;

	abs_path = _which(argv[0], linkedlist_path);
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
		{
			perror("execution failed\n");
			__exit(argv, linkedlist_path);
		}
		
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
      tokens = _realloc(*tokens, token[position], bufsize * sizeof(char*));
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

	linkedlist_path = path_list();

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
				
		if (characters == EOF)
		{
                	free(buffer);
                	continue;
            	}

        } while (1);

return (0);      
}
