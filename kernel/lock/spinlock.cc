#include <kernel/lock/spinlock.h>

using namespace lock;

spinlock::spinlock() {
	lock_flag.flag = false;
}

void spinlock::lock() {
	while(lock_flag.test_and_set());
}

bool spinlock::tryLock() {
	return !lock_flag.test_and_set();
}

void spinlock::unlock() {
	lock_flag.clear();
}
