#include <cstdint.h>
#include <kernel/apps/constructor.h>

extern int main();
extern uint32_t *__app_init_array_start [];
extern uint32_t *__app_init_array_end[];


int apps::callConstructorAndMain() {
	long size = __app_init_array_end - __app_init_array_start;
	for (long i = 0; i < size; i++) {
		/* XXX: Ugly casts and unstable */
		auto tmp = reinterpret_cast<uintptr_t>(__app_init_array_start[i]) >> 32;
		auto fn = reinterpret_cast<void(*)()>(tmp);
		fn();
	}

	return main();
}
