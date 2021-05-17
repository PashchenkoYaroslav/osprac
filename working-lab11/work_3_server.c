#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


int main() {
    int msqid;
    char pathname[] = "work_3_server.c";
    key_t key;
    int msgLen, msgMaxLen;

    struct requestBuf {
        long mtype;
        struct {
            pid_t pid;
            double message;
        } info;
    } clientBuf;

    struct responseBuf {
        long mtype;
        struct {
            double message;
        } info;
    } serverBuf;
    int pid_file = open("work_3_server.c",O_CREAT|O_RDWR,0666);
    int rc = flock(pid_file,LOCK_EX|LOCK_NB);
    if(!rc){
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Failed to generate key\n");
        return(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Failed to get msqid\n");
        return(-1);
    }
    printf("Server has started succesfully\n");
    while (true) {
        msgMaxLen = sizeof(clientBuf.info);
        if (msgrcv(msqid, (struct clientmsgbuf *) &clientBuf, msgMaxLen, -2, 0) < 0) {
            printf("Failed to receive message from queue\n");
            return(-1);
        }
	if(clientBuf.mtype ==2){
            printf("This server was killed!\nShutting down...\n");
	return(-1);
	}
        printf("Received data: Client PID = %d, Data = %lf\n", clientBuf.info.pid, clientBuf.info.message);
        double squareMes = clientBuf.info.message * clientBuf.info.message;
        printf("The response are %lf\n", squareMes);

        serverBuf.mtype = clientBuf.info.pid;
        serverBuf.info.message = squareMes;
        msgLen = sizeof(serverBuf.info);
        if (msgsnd(msqid, (struct sermsgbuf *) &serverBuf, msgLen, 0) < 0) {
            printf("Failed to send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            return(-1);
        }
        printf("Response sent succesfully\n");
    }
}
else{
 printf("Another instance is running\n");
return(-1);
}
    return 0;
}
