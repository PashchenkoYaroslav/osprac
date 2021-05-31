#include <signal.h>

void custom_handler(int sig) {
	if (sig == SIGINT) {
		printf("We got a signal %d.", sig);
		println("Ctrl+C combination will be ignored.");
	}
	if (sig == SIGQUIT) {
	printf("We got a signal %d.", sig);
	println("Ctrl+C combination will be ignored.");
	}
}
int main() {
	signal(SIGINT,custom_handler);
	signal(SIGQUIT,custom_handler);
	while(1);
	return 0;
}
