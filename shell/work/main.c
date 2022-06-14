#include "shell.h"

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
			
		if (_builtin(commands[0]))
			_builtin(commands[0])(commands, linkedlist_path, buffer);
		else
			execute(commands, linkedlist_path);
		
		free(commands);
				
		if (characters == EOF)
		{
                	free(buffer);
                	continue;
            	}

        } while (1);

return (0);      
}
