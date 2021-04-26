#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
Описание работы программы.
В начале работы программы пользователем вводится некое N.
Если оно меньше 1 - выводится соответствующая ошибка.
Затем N раз производятся операции чтения сообщения из pipe и записи в pipe 
Цикл производится следующим образом:
Родитель вызывает функцию D в начале итерации.
Родитель читает сообщение от ребенка.
Родитель записывает сообщения для ребенка.
Родитель вызывает функцию D в конце итерации.
Ребенок вызывает функции Z в начале итерации.
Ребенок читает сообщение от родителя.
Ребенок записывает сообщения для родителя.
Ребенок вызывает функцию A в конце итерации.
*/

void semp(int sem_id, int sem_op) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_op  = sem_op;
  mybuf.sem_flg = 0;

  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Condition error\n");
    return(-1);
  }
}

void A(int sem_id, int value) {
  semp(sem_id, value);
}

void D(int sem_id, int value) {
  semp(sem_id, -value);
}

void Z(int sem_id) {
  semp(sem_id, 0);
}


int main()
{
  int    sem_id;
  char   pathname[] = "work_3_1.c";
  key_t  key;

  int     fd[2], result;
  size_t  size;
  
  char    message[15];
  int     n = 0;


  printf("Enter the value of n. It must be positive digit.\n");
 
  scanf("%d", &n);

  if(n < 1) {
    printf("N is not a positive digit\n");
    return(-1);
  }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    return(-1);
  }

  result = fork();

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    return(-1);
  }

  if ((sem_id = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Error while creating semaphore set\n");
      return(-1);
    } else if ((sem_id = semget(key, 1, 0)) < 0) {
      printf("Missing semaphore\n");
      return(-1);
    }
  } else {
    A(sem_id, 2);
  }

  if (result < 0) {
    printf("Can\'t fork child\n");
    return(-1);
  } else if (result > 0) {

    for (int i = 1; i < n+1; ++i) {
      D(sem_id, 1);

      if (i != 1) {
        size = read(fd[0], message, 15);
        if (size < 0) {
          printf("Can\'t read string from pipe\n");
          return(-1);
        }
        printf("Current iteration : %d\nParent read message : %s\n", i, message);
      }
      size = write(fd[1], "Hello, child!", 14);
      if (size != 14) {
        printf("Can\'t write all string to pipe\n");
        return(-1);
      }

      D(sem_id, 1);
    }

    printf("Parent has closed\n");
  } else {
    for (int i = 1; i <= n; ++i) {
      Z(sem_id);
      size = read(fd[0], message, 14);
      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        return(-1);
      }
      printf("Current iteration : %d\nChild read message : %s\n", i, message);

      size = write(fd[1], "Hello, parent!", 15);
      if (size != 15) {
        printf("Can\'t write all string to pipe\n");
        return(-1);
      }
      A(sem_id, 2);
    }

    if (close(fd[0]) < 0) {
      printf("Can\'t close reading pipe\n");
      return(-1);
    }
    if (close(fd[1]) < 0) {
      printf("Can\'t close writing pipe\n");
      return(-1);
    }
    printf("Child has closed\n");
  }

  return 0;
}
