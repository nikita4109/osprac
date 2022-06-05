#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int recevied = 0;
pid_t pid;

void handler(int signum) {
	if (signum == SIGUSR2) {
		recevied = 1;
	}
}

void send(int bit) {
	if (bit) {
		kill(pid, SIGUSR2);
	} else {
		kill(pid, SIGUSR1);
	}

	while(!recevied);
	recevied = 0;
}

void main() {
	signal(SIGUSR2, handler);

	printf("PID = %d\n", getpid());

	printf("Enter receiver pid = ");
	scanf("%d", &pid);

	while(!recevied);
	recevied = 0;

	int number;
	printf("Enter number = ");
	scanf("%d", &number);

	if (number < 0) {
		number *= -1;
		send(1);
	} else {
		send(0);
	}

	while (number) {
		send(number % 2);
		number /= 2;
	}
}
