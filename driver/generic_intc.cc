#include <cerrno.h>
#include <driver/generic_intc.h>

using namespace driver;

int generic_intc::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

int generic_intc::registerHandler(void* data, size_t size, lib::function<int()> handler) {
	(void) data;
	(void) size;
	(void) handler;
	return -ENXIO;
}

int generic_intc::handleIRQ() {
	return -1;
}
