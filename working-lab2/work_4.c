#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[], char **envp)
{
pid_t ppid, pid;
int this_pid = getpid();
fork();
if(getppid() == this_pid)
{
execle("/bin/date","date",0,envp);
printf("An error occured!");
exit(-1);
}
return 0;
}

