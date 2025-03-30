#include <cstdint.h>
#include <kernel/apps/constructor.h>

extern int main();
extern void (*__app_init_array_start []) ();
extern void (*__app_init_array_end[]) ();


int apps::callConstructorAndMain() {
	long size = __app_init_array_end - __app_init_array_start;
	for (long i = 0; i < size; i++) {
		(*__app_init_array_start[i])();
	}

	return main();
}
