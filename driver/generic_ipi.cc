#include <cerrno.h>
#include <driver/generic_ipi.h>

using namespace driver;

int generic_ipi::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

int generic_ipi::sendIPI(size_t cpuID, size_t msg) {
	(void) cpuID;
	(void) msg;

	return -ENXIO;
}

int registerHandler(size_t msg, lib::function<int()> handler) {
	(void) msg;
	(void) handler;

	return -ENXIO;
}

lib::pair<void*, size_t> generic_ipi::getConfigSpace() const {
	return lib::pair(nullptr, 0);
}
