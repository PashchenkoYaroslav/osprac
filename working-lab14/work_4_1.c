
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int sender_pid;
int index = 0;
int value;
bool received = false;

void sigusr1_handler_bit(int nsig) {
	value |= (1 << index++);
	kill(sender_pid, SIGUSR1);
}

void sigusr2_handler_nobit(int nsig) {
	index++;
	kill(sender_pid, SIGUSR1);
}

void sigchld_handler(int nsig) {
	received = true;
}

int main(void) {
  printf("Receiver PID is %d\n", getpid());

  printf("Enter sender's PID : ");
  scanf("%d", &sender_pid);

  signal(SIGUSR1, sigusr1_handler_bit);
  signal(SIGUSR2, sigusr2_handler_nobit);
  signal(SIGCHLD, sigchld_handler);

  while (!received);

  printf("This program received a message : %d\n", value);
  printf("Receiver has successfully completed its task\n");
  return 0;
}

