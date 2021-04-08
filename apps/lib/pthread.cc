#include <pthread.h>

pthread_t pthread_self(void) {
	pthread_t ret = 0;
	asm("mrs %0, TPIDR_EL0" : "=r"(ret));
	return ret;
}
