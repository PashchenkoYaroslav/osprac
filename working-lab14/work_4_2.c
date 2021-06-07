#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int receiver_pid;
int value;
bool can_send = true;

void receiver_signal_handler(int nsig) {
	can_send = true;
}

void send_value() {
	int bits_count = sizeof(int) * 8;
	for (int i = 0; i < bits_count; ++i) {
		while (!can_send);
		if ((value & (1 << i)) != 0) {
			kill(receiver_pid, SIGUSR1);
		}
		else {
			kill(receiver_pid, SIGUSR2);
		}
		can_send = false;
	}
	kill(receiver_pid, SIGCHLD);
}

int main(void) {
  signal(SIGUSR1, receiver_signal_handler);
  printf("Sender PID is %d\n", getpid());

  printf("Enter receiver's PID : ");
  if (scanf("%d", &receiver_pid) < 0) {
	printf("An error occurred while reading receiver PID.\n");
	exit(-1);
  }
	
  printf("Enter a message that is an integer : ");
  if (scanf("%d", &value) < 0) {
	printf("An error occurred while reading the number.\n");
	exit(-2);
  }

  send_value();

  printf("Sender has successfully completed its task\n");
  return 0;
}
