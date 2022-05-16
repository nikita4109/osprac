#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

struct sembuf sem; // Structure for specifying operations on a semaphore.
int    semid;        // IPC descriptor for an array of IPC semaphores

void up() {
  sem.sem_num = 0;
  sem.sem_op  = 1;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

void down() {
  sem.sem_num = 0;
  sem.sem_op  = -1;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

void wait() {
  sem.sem_num = 0;
  sem.sem_op  = 0;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

void init() {
  char   pathname[]="task11-3-server.c"; // The file name used to generate the key.
                               // A file with this name must exist in the current directory
  key_t  key;          // IPC key

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Try to access by key the array of semaphores, if it exists,
  // or create it from a single semaphore if it does not exist yet,
  // with read & write access for all users.
  //
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
}

struct mymsgbuf // Custom structure for the message
{
  long mtype;
  float data;
  pid_t pid;
} mybuf;

int msqid;      // IPC descriptor for the message queue

void send() {
  mybuf.mtype = mybuf.pid;
  mybuf.data *= mybuf.data;

  int len = sizeof(mybuf) - sizeof(mybuf.mtype);
  
  //
  // Send the message. If there is an error,
  // report it and delete the message queue from the system.
  //
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
}

int main(void)
{
  char pathname[]= "task11-3-server.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  init();
  wait();
  up();

  while (1) {
    //
    // In an infinite loop, accept messages of any type in FIFO order
    // with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    //
    int maxlen = sizeof(mybuf) - sizeof(mybuf.mtype);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    
    if (mybuf.pid == 1) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      printf("Server was killed\n");
      exit(0);
    }

    send();
  }

  down();

  return 0;
}
