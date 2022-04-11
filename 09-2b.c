#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>

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

int main()
{
  int     *array;
  int     shmid, semid;
  int     new = 1;
  char    pathname[] = "09-2a.c";
  key_t   key, skey;
  long    i;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  struct sembuf mybuf;

  if ((skey = ftok(pathname,1)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  if (new) {
    up(&mybuf, semid);
  }

  down(&mybuf, semid);
  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    array[1] += 1;
    for(i=0; i<2000000000L; i++);
    array[2] += 1;
  }

  up(&mybuf, semid);

  printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
