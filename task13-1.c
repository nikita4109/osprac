#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    FILE* file_ptr = fopen("regular_file", "w");
    fprintf(file_ptr, "%s", "regular file");
    fclose(file_ptr);

	int depth = 0;
	char sym_name[20];
	sprintf(sym_name, "%d", depth);

	char prev_name[20] = "regular_file";
	while (symlink(prev_name, sym_name) == 0 && fopen(sym_name, "r") != NULL) {
		++depth;
		for (int i = 0; i < 20; ++i) {
			prev_name[i] = sym_name[i];
		}

		sprintf(sym_name, "%d", depth);
	}

	printf("Max recursion depth: %d\n", depth);

	return 0;
}
