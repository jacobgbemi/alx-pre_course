#include "shell.h"

/**
 * env_linked_list - creates a linked list from environmental variables
 * @env: environmental variables
 * Return: linked list
 */
list_t *env_linked_list(char **env)
{
	list_t *head;
	int i = 0;

	head = NULL;
	while (env[i] != NULL)
	{
		add_end_node(&head, env[i]);
		i++;
	}
	return (head);
}

int main(void)
{
	list_t *head;

	head = NULL;
	char p[12] = "PATH";
	char *path = p;
	env_linked_list(**path);
	print_list(head);

	return (0);
	
}
