#include <dirent.h> 
#include <stdio.h> 

char *current_dir(void)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return(NULL);
}

int main()
{
	current_dir();
	return (0);
}
