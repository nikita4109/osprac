#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

  while (1) {
    //
    // In an infinite loop, accept messages of any type in FIFO order
    // with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    //
    int maxlen = sizeof(mybuf) - sizeof(mybuf.mtype);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
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

  return 0;
}
