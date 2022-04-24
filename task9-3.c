#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void wait(struct sembuf* mybuf, int semid) {
    mybuf->sem_num = 0;
    mybuf->sem_op  = 0;
    mybuf->sem_flg = 0;

    if (semop(semid, mybuf, 1) < 0) {
      printf("Can\'t wait for condition\n");
      exit(-1);
    }
}

void up(struct sembuf* mybuf, int semid) {
    mybuf->sem_num = 0;
    mybuf->sem_op  = 1;
    mybuf->sem_flg = 0;

    if (semop(semid, mybuf, 1) < 0) {
      printf("Can\'t wait for condition\n");
      exit(-1);
    }
}

void down(struct sembuf* mybuf, int semid) {
    mybuf->sem_num = 0;
    mybuf->sem_op  = -1;
    mybuf->sem_flg = 0;

    if (semop(semid, mybuf, 1) < 0) {
      printf("Can\'t wait for condition\n");
      exit(-1);
    }
}

int main() {
    int fd[2], result, semid, count;
    char pathname[] = "task9-3.c", resstring[14];
    key_t key;
    struct sembuf mybuf;
    size_t size;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can't create semaphore set\n");
        exit(-1);
    }

    if (pipe(fd) < 0) {
        printf("Can't open pipe\n");
        exit(-1);
    }

    scanf("%d", &count);
    result = fork();

    if (result < 0) {
        printf("Can't fork child\n");
        exit(-1);
    } else if (result > 0) {
        for (int i = 0; i < count; ++i) {
            size = write(fd[1], "Hello world!", 14);

            if (size != 14) {
                printf("Can't write all string to pipe\n");
                exit(-1);
            }

			up(&mybuf, semid);
			up(&mybuf, semid);

			wait(&mybuf, semid);

            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Can't read string from pipe\n");
                exit(-1);
            }

            printf("Parent got messange:\"%s\"\n", resstring);
        }
    } else {
        for (int i = 0; i < count; ++i) {
			down(&mybuf, semid);

            size = read(fd[0], resstring, 14);

            if (size < 0) {
                printf("Can't read string from pipe\n");
                exit(-1);
            }

            printf("Child got messange:\"%s\"\n", resstring);

            size = write(fd[1], "Hello world!", 14);

            if (size != 14) {
                printf("Can't write all string to pipe\n");
                exit(-1);
            }

			down(&mybuf, semid);
        }
    }

    return 0;
}
