#include <cerrno.h>
#include <kernel/error.h>
#include <driver/generic_intc.h>

using namespace driver;

int generic_intc::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

int generic_intc::registerHandler(void* data, size_t size, generic_driver* driver) {
	(void) data;
	(void) size;
	(void) driver;
	return -ENXIO;
}

generic_driver* generic_intc::getHandler() const {
	return makeError<generic_driver*>(-ENXIO);
}
