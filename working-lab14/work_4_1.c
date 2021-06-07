#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value;
int mask;

void set_zero(int nsig) {
  mask <<= 1;
}

void set_one(int nsig) {
  value |= mask;
  mask <<= 1;
}

int main(void) {
  pid_t sender_pid;
  int   i;

  value = 0;
  mask  = 1;
  (void) signal(SIGUSR1, set_zero);
  (void) signal(SIGUSR2, set_one);

  printf("Receiver PID is %d\n", getpid());

  printf("Enter sender's PID : ");
  scanf("%d", &sender_pid);

  while (1) {
    if(mask == 0) {
      printf("This program received a message : %d\n", value);
      break;
    }
  }

  printf("Receiver has successfully completed its task\n");
  return 0;
}
