#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fd[2];

	if (pipe(fd) < 0) {
		printf("Can\'t open pipe\n");
		exit(-1);
	}

	fcntl(fd[1], F_SETFL, O_NONBLOCK);

	int pipe_size = 0;
	while (write(fd[1], "h", 1) == 1) {
		++pipe_size;
	}

	printf("Size of pipe in bytes = %d\n", pipe_size);

	return 0;
}
