#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
int main()
{
// Данный файл принимает сообщение и печатает его.
// Для успешной работы программы необходимо сперва сперва скомпилировать work_4_2,
// запустить его, затем скомпилировать work_4_1 и запустить его.
int fd;
size_t size;
char resstring[14];
char name[] = "f.fifo";

(void)umask(0);

if((fd = open(name,O_RDONLY))<0){
printf("Error while opening file\n");
exit(-1);
}
size = read(fd,resstring,14);

if(size<0){
printf("Error while reading file\n");
exit(-1);
}

printf("Result: %s\n",resstring);
if(close(fd)<0){
printf("Error while closing file\n");
exit(-1);
}
return 0;
}

