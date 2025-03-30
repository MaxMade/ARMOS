#include <errno.h>
#include <unistd.h>

int main(void) {
	/* Perform invalid write system call */
	write(0, nullptr, 0);
	return errno;
}
