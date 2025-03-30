#include <unistd.h>
#include <sys/syscall.h>

ssize_t write(int fd, const void *buf, size_t count) {
	return syscall(SYS_WRITE, fd, buf, count);
}
