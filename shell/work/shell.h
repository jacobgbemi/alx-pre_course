#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <wait.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

#define BUFSIZE 1024
extern char **environ;

typedef struct builtin_commands
{
	char *cmd_str;
	int (*fun)();
} builtin_cmds_t;


/**
 * struct list - linked list for environmental variables
 * @var: holds environmental variable string
 * @next: points to next node
 */
typedef struct env_path
{
	char *str;
	unsigned int len;
	struct env_path *next;

} list_t;

/* In builtins.c */
int (*is_builtin(char *cmd))();
int _exit_with_grace(char **tokens, list_t *linkedlist_path, char *buffer);
int _env(char **tokens, list_t *environment);
int _cd(char **tokens);

/* In builtins_2.c */
int _setenv_usr(char **tokens);
int _alias(void);
int _history(void);

list_t *add_node(list_t **head, char *str, unsigned int len);
list_t *list_from_path(void);
list_t *environ_linked_list(void);
char *search_os(char *cmd, list_t *linkedlist_path);
void executor(char *argv[], list_t *linkedlist_path);
char *_strcpy(char *dest, char *src);
char **split_line(char *line);
void free_double_ptr(char **str);
void free_list(list_t *head);

/* strings functins*/
int _putchar(char c);
void _puts(char *str);
int _isnumber(int c);
int _strlen( char *s);

/* strtok */
char *_strchr(char *s, char c);
unsigned int _strspn(char *s, char *accept);
char *_strpbrk(char *s, char *delims);
char *_strtok(char *s, char *delim);

/* helper functions*/
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

#endif
