#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main (int argc, char *argv[], char **envp)
{
while(*envp)
	printf("%s\n",*envp++);
return 0;
}


