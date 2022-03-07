#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char *text;
    int size;
    int fd;
    int shmid;
    char pathname[] = "task7-1w.c";
    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((fd = open(pathname, O_RDONLY)) < 0) {
        printf("Can\'t open file for read");
        exit(-1);
    }

    size = lseek(fd, 0, SEEK_END);

    if ((shmid = shmget(key, 0, 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
    }

    if ((text = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

	printf("%s", text);
//    for (int i = 0; i < size; ++i) {
//        printf("%c", text[i]);
//    }

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
