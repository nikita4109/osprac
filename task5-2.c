#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd_first[2], fd_second[2], result;

  size_t size;
  char  resstring[30];

  if (pipe(fd_first) < 0 || pipe(fd_second) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fd_first[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
    }

    size = write(fd_first[1], "Hello, world!", 14);

    if (size != 14) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }

    if (close(fd_first[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
    }

	// Parent reading from second pipe

    if (close(fd_second[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
    }

    size = read(fd_second[0], resstring, 29);

    if (size < 0) {
      printf("Can\'t read string from pipe\n");
      exit(-1);
    }

    printf("Parent exit, resstring:%s\n", resstring);

    if (close(fd_second[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
    }
  } else {

    /* Child process */

	// Child reading from first pipe

    if (close(fd_first[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }

    size = read(fd_first[0], resstring, 14);

    if (size < 0) {
      printf("Can\'t read string from pipe\n");
      exit(-1);
    }

    printf("Child exit, resstring:%s\n", resstring);

    if (close(fd_first[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }

	// Child writing to second pipe

    if (close(fd_second[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }

    size = write(fd_second[1], "Hello, world to second pipe!", 29);

    if (size != 29) {
      printf("Can\'t write all string to second pipe\n");
      exit(-1);
    }

    if (close(fd_second[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }
  }

  return 0;
}
