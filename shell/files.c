#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFSIZE 1024

/**
  * struct environ_path - linked list from PATH
  * @path: path in the format /usr/bin
  * @len: length of the string
  * @next: points to the next node
  */
typedef struct environ_path
{
	char *str;
	unsigned int len;
	struct environ_path *next;
} env_path_t;

/**
  * add_node - adds a new node to the end of a linked list
  * @head: head of the linked list
  * @str: string
  * @len: length of the string
  * Return: pointer to the current position in the list
  **/
env_path_t *add_node(env_path_t **head, char *str, unsigned int len)
{
	env_path_t *new, *walk;
	char *dupstr;

	if (str == NULL)
		return (NULL);
	dupstr = strdup(str);
	if (dupstr == NULL)
		return (NULL);
	new = malloc(sizeof(env_path_t));
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
	walk = *head;
	while (walk->next != NULL)
		walk = walk->next;
	walk->next = new;
	return (*head);
}

/**
  * list_from_path - builds a linked list from PATH
  * Return: pointer to linked list
  */
env_path_t *list_from_path(void)
{
	unsigned int len, i, j;
	char *env;
	char buffer[BUFSIZE];
	env_path_t *ep;

	ep = NULL;
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
			add_node(&ep, buffer, len);
			len = j = 0;
		}
		env++;
	}
	return (ep);
}


char *current_dir(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return(0);
}



char *search_os(char *cmd)
{
	char *abs_path;
	env_path_t *ep;

	ep = list_from_path();
	if (ep == NULL)
		perror("List empty.\n"), exit(99);
	/*
	print_list(ep);
	*/

	while (ep)
	{
		abs_path = strdup(ep->str);
		abs_path = strcat(abs_path, cmd);
		/*
		printf("abs_path: %s\n", abs_path);
		*/
		if (access(abs_path, F_OK | X_OK) == 0)
		{
			/*
			printf("found it\n");
			*/
			abs_path = current_dir();
			return (abs_path);
		}
		ep = ep->next;
	}
	/*
	printf("Didn't find it\n");
	*/
	return (NULL);
}

/**
  * main - Entry point
  * Return: 0 on success
  */

/*int main(int ac, char *av[])
*{
*	char *path;
*	if (ac != 2)
*		perror("Usage: ./a.out <command_to_find>"), exit(100);
*	path = search_os(av[1]);
*	if (path)
*		printf("%s\n", path);
*	else
*		printf("Did not find command\n");
*	return (0);
*}
*/

int main(int ac, char *av[])
{
	char *buffer, *cmd, *path;
	size_t len = 1024;
	int status;
	pid_t child_pid;
	
	buffer = malloc(sizeof(size_t) * len);
	path = search_os(av[0]);
	

	while (1)
	{
		printf("$ ");
		getline(&buffer, &len, stdin);
		/*if (read != EOF)
		*	buffer = argv[ac++];*/
		
		while (ac >= 1)
		{
			child_pid = fork();
			if (child_pid == -1)
				return (1);
			if (child_pid == 0)
			{
				cmd = strdup(path);
				printf("%s", cmd);		
			}		
			else
				wait(&status);
			/*free (cmd);*/

		}

	}

	return (0);


}
