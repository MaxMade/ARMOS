#include <unistd.h>

extern "C" int main(void) {

	/* Perform write system call */
	const char* msg =  "Hallo Welt!";
	size_t len = 11;
	syscall(1, 1, msg, len);

	while (1);
	return 0;
}
