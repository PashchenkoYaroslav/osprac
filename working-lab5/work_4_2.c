#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
int main()
{
// Данный файл создает FIFO.
// Для успешной работы программы необходимо сперва сперва скомпилировать work_4_2,
// запустить его, затем скомпилировать work_4_1 и запустить его.
int fd;
size_t size;
char name[] = "f.fifo";
char mes[] = "Hello, world!";

(void)umask(0);

if(mknod(name, S_IFIFO|0666,0)<0){
printf("Error while creating FIFO\n");
exit(-1);
}

if((fd = open(name,O_WRONLY))<0){
printf("Error while opening FIFO\n");
}

size = write(fd,mes,14);

if(size !=14){
printf("Error while writing FIFO\n");
exit(-1);
}

if(close(fd)<0){
printf("Error while closing FIFO\n");
}

return 0;
}
