#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

#define LAST_MESSAGE 255

int main() {
	int msqid;

	char pathname[] = "work_1_1.c";

	key_t key;

	int len, maxlen;

	struct mymsgbuf {
		long mtype;
		struct {
			short sinfo;
			float finfo;
		} info;
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can't generate key\n");
		return(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
		printf("Can't get msqid\n");
		return(-1);
	}

	while (1) {
	maxlen = sizeof(mybuf.info);
	if (len = msgrcv(msqid, &mybuf, maxlen, 0, 0) < 0) {
		printf("Can't receive message from queue\n");
		return(-1);
	}

	if (mybuf.mtype == LAST_MESSAGE) {
		msgctl(msqid, IPC_RMID, NULL);
		return(0);
	}

	printf("Message type : %ld, sInfo : %i, fInfo : %f\n",
		mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
	}

	return 0;
}
