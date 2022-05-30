#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
  if (nsig == SIGQUIT) {
    printf("Receive signal %d, CTRL-4 pressed\n", nsig);
  } else {
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
  }
}

int main(void) {
  (void)signal(SIGINT, my_handler);
  (void)signal(SIGQUIT, my_handler);

  while(1);
  return 0;
}
