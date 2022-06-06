#include <stdio.h>
#include <stdlib.h>

char *_getenv(const char *name)
{

}

int main () {
   printf("PATH : %s\n", getenv("PATH"));
   printf("HOME : %s\n", getenv("HOME"));
   printf("ROOT : %s\n", getenv("ROOT"));

   return(0);
}
