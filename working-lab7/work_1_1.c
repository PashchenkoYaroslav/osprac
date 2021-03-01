#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
int main()
{
    int *array;
    int shmid;
    char pathname[] = "work_1_1.c";
    key_t key;
    char *src;
    
    int file_size = 0;
    int *file_size_ptr;
    
    if ((key = ftok(pathname,0)) < 0) 
    {
        printf("Key generation error \n");
        exit(-1);
    }
    
    if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0)) < 0)  
    {
        printf("Memory creation error \n");
        exit(-1);
    }

    if ((file_size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) 
    {
        printf("Memory attaching error \n");
        exit(-1);
    }

    file_size = *file_size_ptr;
    src = (char*)(file_size_ptr + 1);
    
    for (int i = 0; i < file_size; i++)
        putchar(src[i]);
    
    if (shmdt(file_size_ptr) < 0)   
    {
        printf("Memory detaching error \n");
        exit(-1);
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) < 0) 
    {
        printf("Memory delete error \n");
        exit(-1);
    }

    return 0;
}
