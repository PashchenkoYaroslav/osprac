#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
int counter = 0;

void *mythread(void *dummy)
{
    pthread_t mythid;
    mythid = pthread_self();
    counter = counter + 1;
    printf("Thread %d, Calculation result = %d\n", mythid, counter);
    return NULL;
}

int main()
{
    pthread_t thid, thid2, mythid;
    int result;

    result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, NULL);
    if (result != 0) {
        printf ("Thread creation error, return value = %d\n", result);
        exit(-1);
    }
    printf("2nd thread created, thid = %d\n", thid);

    result = pthread_create( &thid2, (pthread_attr_t *)NULL, mythread, NULL);
    if (result != 0) {
        printf ("Thread creation error, return value = %d\n", result);
        exit(-1);
    }
    printf("3rd thread created, thid = %d\n", thid2);

    mythid = pthread_self();
    counter = counter + 1;
    printf("Thread %d, Calculation result = %d\n", mythid, counter);

    pthread_join(thid, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
    return 0;
}
