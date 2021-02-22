#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

int size = 0;
int kb_size = 0;
int main() {
int fd[2];
if(pipe(fd)<0){
printf("Error while creating pipe\n");
return -1;
}
fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFD)| O_NONBLOCK);
while(write(fd[1],"1",1)==1){
size++;
}
// Тут делим на 1024 чтобы получить количество килобайтов.
kb_size = size/1024;
printf("Pipe size is %d in kilobytes\n", kb_size);
return 0;
}
