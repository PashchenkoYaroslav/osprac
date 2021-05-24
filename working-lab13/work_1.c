#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_NAME_SIZE 32

void get_max_depth(int recursive_depth) {
	char previous_file_name[BUFFER_NAME_SIZE];
	char current_file_name[BUFFER_NAME_SIZE];

	sprintf(previous_file_name, "test_number_%d", recursive_depth);
	sprintf(current_file_name, "test_number_%d", recursive_depth + 1);

	if (symlink(previous_file_name, current_file_name) != 0) {
		printf("Error while creating symlink.\n");
		exit(-4);
	}
	++recursive_depth;
	int file_descriptor;
	if ((file_descriptor = open(current_file_name, O_RDWR | O_CREAT, 0666)) < 0) {
		printf("Calculation is over.\n");
		printf("Max possible depth is %d\n", recursive_depth - 1);
		if (remove(current_file_name) < 0) {
			printf("Error while removing a link.\n");
			exit(-5);
		}
		return;
	}
	if (close(file_descriptor) < 0) {
		printf("Error while closing the file.\n");
		exit(-3);
	}

	get_max_depth(recursive_depth);
	if (remove(current_file_name) < 0) {
		printf("Error while removing a link.\n");
		exit(-5);
	}
}

int main() {
	char file_name[] = "test_number_0";
	int file_descriptor;

	if ((file_descriptor = open(file_name, O_RDWR | O_CREAT, 0666)) < 0) {
		printf("Error while opening the file.\n");
		exit(-1);
	}

	if (close(file_descriptor) < 0) {
		printf("Error while closing the file.\n");
		exit(-2);
	}
        printf("Calculation is starting...\n");
	get_max_depth(0);
	if (remove(file_name) < 0) {
		printf("Error while removing the initial file.\n");
		exit(-6);
	}

	return 0;
}
