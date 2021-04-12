#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

int main()
{

    int semid;

    char pathname[] = "work_1_1.c";

    key_t key;

    struct sembuf mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        return(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get semid\n");
        return(-1);
    }

    mybuf.sem_op = -5;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        return(-1);
    }

    printf("Condition is present\n");
    return 0;
}
