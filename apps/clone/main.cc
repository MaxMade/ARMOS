#include <sched.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static char integers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static const size_t STACK_ALIGN = 16;

static const size_t STACK_SIZE = 4096 * 16;

static char stack[STACK_SIZE] __attribute__((aligned(4096)));

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
	if (clone(fn, &stack[STACK_SIZE - STACK_ALIGN], CLONE_VM | CLONE_THREAD, (void*) "Hello World\n\r") == -1)
		return -1;

	while (1) {
		char tmp[] = {integers[pthread_self() % 10], '\n', '\r'};
		write(STDOUT_FILENO, tmp, 3);
	}

	return 0;
}
