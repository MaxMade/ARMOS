#include <unistd.h>
#include <pthread.h>

static char integers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

extern "C" int main(void) {

	/* Print thread ID */
	syscall(1, 1, "Thread ID: ", 11);
	auto id = pthread_self();
	syscall(1, 1, &integers[id], 1);
	syscall(1, 1, "\n\r", 2);

	while(1);

	return 0;
}
