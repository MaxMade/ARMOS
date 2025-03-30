#include <cerrno.h>
#include <driver/generic_timer.h>

using namespace driver;

int generic_timer::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}


int generic_timer::windup(size_t ms) {
	(void) ms;
	return -ENXIO;
}

size_t generic_timer::interval() const {
	return -ENXIO;
}

size_t generic_timer::getTicks() const {
	return -ENXIO;
}

int generic_timer::registerFunction(size_t ms, lib::function<int(void)> callback) {
	(void) ms;
	(void) callback;

	return -ENXIO;
}
