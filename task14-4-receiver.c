#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int number = 0, count = -1, sign = 1;
pid_t pid;

void handler(int signum) {
	if (signum == 0) {
		return;
	}

	int bit = 0;
	if (signum == SIGUSR2) {
		bit = 1;
	}

	if (count == -1) {
		if (bit) {
			sign = -1;
		}
	} else {
		number += bit << count;
	}

	++count;

	// Sender can send next bit.
	kill(pid, SIGUSR2);
}

void main() {
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);

	printf("PID = %d\n", getpid());

	printf("Enter sender pid = ");
	scanf("%d", &pid);

	// Sender can start sending number.
	kill(pid, SIGUSR2);

	// Wait until sender ends.
	while(kill(pid, 0) == 0);

	printf("Received number %d\n", sign * number);
}
