#include <sched.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static char integers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static const size_t STACK_ALIGN = 16;

static const size_t STACK_SIZE = 4096 * 16;

static char stack[8][STACK_SIZE] __attribute__((aligned(4096)));

static int fn(void* msg) {
	auto o = reinterpret_cast<const char*>(msg);
	write(STDOUT_FILENO, o, strlen(o));

	while (1) {
		char tmp[] = {integers[pthread_self() % 10], '\n', '\r'};
		write(STDOUT_FILENO, tmp, 3);
	}

	return 0;
}

extern "C" int main(void) {
	if (clone(fn, &stack[0][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 0\n\r") == -1)
		return -1;

	if (clone(fn, &stack[1][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 1\n\r") == -1)
		return -1;

	if (clone(fn, &stack[2][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 2\n\r") == -1)
		return -1;

	if (clone(fn, &stack[3][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 3\n\r") == -1)
		return -1;

	if (clone(fn, &stack[4][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 4\n\r") == -1)
		return -1;

	if (clone(fn, &stack[5][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 5\n\r") == -1)
		return -1;

	if (clone(fn, &stack[6][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 6\n\r") == -1)
		return -1;

	if (clone(fn, &stack[7][STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World 7\n\r") == -1)
		return -1;

	fn((void*) "Hello World 4\n\r");

	return 0;
}
