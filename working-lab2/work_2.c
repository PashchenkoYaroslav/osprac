#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main ()
{
pid_t pid = fork();

if(pid == -1){
printf("An error occures!");
}
else if(pid == 0){
printf("It is a child");
}
else{
printf("It is a parent");
}

printf("\nPid = %d\nPpid = %d\n",getpid(), getppid());

return 0;
}

