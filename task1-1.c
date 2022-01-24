#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
	uid_t user_id = getuid();
	gid_t group_id = getgid();

	printf("User id - %d, group id - %d\n", user_id, group_id);

    return 0;
}
