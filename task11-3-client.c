#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]= "task11-3-server.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    float data;
    pid_t pid;
  } mybuf;

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

  /* Send information */

  printf("Enter float\n");
  scanf("%f", &mybuf.data);

  mybuf.mtype = 1;
  mybuf.pid = getpid();
  len = sizeof(mybuf) - sizeof(mybuf.mtype);

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  int maxlen = sizeof(mybuf) - sizeof(mybuf.mtype);
  if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, mybuf.pid, 0)) < 0) {
    printf("Can\'t receive message from queue\n");
    exit(-1);
  }

  printf("%f\n", mybuf.data);

  return 0;
}
