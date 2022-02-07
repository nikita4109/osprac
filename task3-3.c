#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
	for (int i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}

	for (int i = 0; envp[i] != NULL; ++i) {
		printf("%s\n", envp[i]);
	}

	return 0;
}
