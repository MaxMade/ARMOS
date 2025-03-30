#include <errno.h>
#include <pthread.h>

pthread_t pthread_self(void) {
	pthread_t ret = 0;
	asm("mrs %0, TPIDR_EL0" : "=r"(ret));
	return ret;
}

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
	(void) pshared;
	*lock = false;
	return 0;
}

int pthread_spin_destroy(pthread_spinlock_t *lock) {
	(void) lock;
	return 0;
}

int pthread_spin_lock(pthread_spinlock_t *lock) {
	while(__atomic_test_and_set(lock, __ATOMIC_SEQ_CST));
	return 0;
}

int pthread_spin_trylock(pthread_spinlock_t *lock) {
	bool locked = __atomic_test_and_set(lock, __ATOMIC_SEQ_CST);

	return locked ? EBUSY : 0;
}

int pthread_spin_unlock(pthread_spinlock_t *lock) {
	__atomic_clear(lock, __ATOMIC_SEQ_CST);
	return 0;
}
