#include <unistd.h>

static char integers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

extern "C" int main(void) {

	/* Perform write system call */
	const char* msg =  "Hallo Welt!\n\r";
	size_t len = 13;
	syscall(1, 1, msg, len);

	for (int i = 0; true; i++) {
		/* Increment counter */
		char counting[] = "0\n\r";
		counting[0] = integers[i % 10];
		syscall(1, 1, counting, 3);
	}
	return 0;
}
