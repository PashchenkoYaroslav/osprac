#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int P(int semid, struct sembuf *buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int V(int semid, struct sembuf *buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int main() {
    int semid;

    struct sembuf mybuf;

    int *array; 

    int shmid;

    int new = 1; 

    char pathname[] = "work_2_1.c";

    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        return(-1);
    }

    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can\'t create shared memory\n");
            return(-1);
        } else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can\'t find shared memory\n");
                return(-1);
            }
            new = 0;
        }
    }

    if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        return(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not found. Attempt to create one...\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get semid\n");
            return(-1);
        }
        printf("Create successful!\n");
        V(semid, &mybuf);
    }

    if (new) {
        array[0] = 1;
        array[1] = 0;
        array[2] = 1;
    } else {
        P(semid, &mybuf);
        array[0] += 1;
        for (long i = 0; i < 1000000000L; i++);
        array[2] += 1;
        V(semid, &mybuf);
    }

    printf("Work_2_1 was executed %d times, Work_2_2 - %d times, Total number is %d times\n",
           array[0], array[1], array[2]);

    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        return(-1);
    }
    return 0;
}
