#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
  pid_t receiver_pid;
  int   input, mask;
  int   i;

  printf("Sender PID is %d\n", getpid());

  printf("Enter receiver's PID : ");
  scanf("%d", &receiver_pid);

  printf("Enter a message that is an integer: ");
  scanf("%d", &input);

  mask = 1;
  while (mask != 0) {
    if (input & mask) {
      kill(receiver_pid, SIGUSR2);
    } else {
      kill(receiver_pid, SIGUSR1);
    }
    mask <<= 1;
    for (i = 0; i < 30000000; ++i);
  }

  printf("Sender has successfully completed its task\n");
  return 0;
}
