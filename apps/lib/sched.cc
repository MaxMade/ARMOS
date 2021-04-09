#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>

int clone(int (*fn)(void *), void *stack, int flags, void* arg) {
	return syscall(SYS_CLONE, fn, stack, flags, arg);
}
