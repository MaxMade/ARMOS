#include <errno.h>
#include <unistd.h>

extern "C" int main(void) {
	/* Perform invalid write system call */
	write(0, nullptr, 0);
	return errno;
}
