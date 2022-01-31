#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	pid_t pid = fork();

	if (pid == -1) {
		printf("Something went wrong!\n");
	} else if (pid == 0) {
		printf("This is child, pid = %d\n", getpid());
	} else {
		printf("This is parent, pid = %d\n", getpid());
	}

	return 0;
}
