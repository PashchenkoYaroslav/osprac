#include <signal.h>
#include <stdio.h>
void custom_handler(int sig) {
	if (sig == SIGINT) {
		printf("We got a signal %d.", sig);
		printf("Ctrl+C combination will be ignored.\n");
	}
	if (sig == SIGQUIT) {
	printf("We got a signal %d.", sig);
	printf("Ctrl+4 combination will be ignored.\n");
	}
}
int main() {
	signal(SIGINT,custom_handler);
	signal(SIGQUIT,custom_handler);
	while(1);
	return 0;
}
