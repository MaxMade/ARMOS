#include <pthread.h>

int main(void) {
	pthread_spinlock_t lock;
	pthread_spin_init(&lock, 0);
	pthread_spin_lock(&lock);
	pthread_spin_trylock(&lock);
	pthread_spin_unlock(&lock);
	pthread_spin_destroy(&lock);
	return 0;
}
