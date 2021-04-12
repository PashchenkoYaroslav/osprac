#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     semid;        
  struct  sembuf mybuf;
  int     *array;
  int     shmid;
  int     new = 1; 
  char    pathname[] = "work_2_1.c";
  key_t   key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    return(-1);
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      return(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        return(-1);
      }
      new = 0;
    }
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
      if (errno != EEXIST) {
          printf("The semaphore does not exist.\n");
          return(-1);
      }
      else {
          if ((semid = semget(key, 1, 0)) < 0) {
              printf("Can\'t get semaphore by key\n");
              return(-1);
          }
      }
  }
  else {
      mybuf.sem_num = 0;
      mybuf.sem_op = 1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
          printf("Can\'t set value\n");
          return(-1);
      }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    return(-1);
  }

  mybuf.sem_num = 0;
  mybuf.sem_op = -1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
      printf("Can\'t enter the critical section\n");
      return(-1);
  }

  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    array[1] += 1;
    for(long i=0; i<2000000000L; i++);
    array[2] += 1;
  }

  printf
    ("Work_2_1 was called %d times, Work_2_2 was called %d times, total number of calls is %d times\n",
    array[0], array[1], array[2]);

  mybuf.sem_num = 0;
  mybuf.sem_op = 1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
      printf("Can\'t exit the critical section\n");
      return(-1);
  }

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    return(-1);
  }

  return 0;
}
