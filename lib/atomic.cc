#include <atomic.h>

using namespace lib;

void atomic_flag::clear(memory_order order) {
	__atomic_clear(&flag, order);
}

bool atomic_flag::test_and_set(memory_order order) {
	return __atomic_test_and_set(&flag, order);
}

bool atomic_flag_test_and_set(atomic_flag* p) {
	return p->test_and_set();
}

bool atomic_flag_test_and_set_explicit(atomic_flag* p, memory_order order) {
	return p->test_and_set(order);
}


void atomic_flag_clear(atomic_flag* p) {
	p->clear();
}

void atomic_flag_clear_explicit(atomic_flag* p, memory_order order) {
	p->clear(order);
}
