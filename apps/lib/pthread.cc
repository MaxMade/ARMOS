#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

static const int PTHREAD_STACK_SIZE = 16 * 4096;

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

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
	(void) attr;

	char* stack = reinterpret_cast<char*>(malloc(PTHREAD_STACK_SIZE));
	if (stack == nullptr) {
		return EAGAIN;
	}

	int id = clone((int (*)(void*)) (void*) start_routine, &stack[PTHREAD_STACK_SIZE - 16], CLONE_VM | CLONE_THREAD, arg);
	if (id == -1) {
		return EAGAIN;
	}

	*thread = id;

	return 0;
}
