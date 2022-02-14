#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	int     fd;
	size_t  size;
	char    result[14];

	(void)umask(0);

	if ((fd = open("myfile", O_RDONLY | O_CREAT, 0666)) < 0) {
		printf("Can\'t open file\n");
		exit(-1);
	}

	size = read(fd, result, 14);

	if (size != 14) {
		printf("Can\'t read all bytes\n");
		exit(-1);
	}

	printf("%s\n", result);

	if (close(fd) < 0) {
		printf("Can\'t close file\n");
	}

  return 0;
}
