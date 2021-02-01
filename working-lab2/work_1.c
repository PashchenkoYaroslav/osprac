#include <sys/types.h>
#include <unistd.h>
int main (void)
{
pid_t pid,ppid;

pid = getpid();
ppid = getppid();

printf("Pid = %d\n",pid);
printf("Ppid = %d\n",ppid);
return 0;
}

