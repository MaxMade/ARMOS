#include <cerrno.h>
#include <driver/generic_ipi.h>

using namespace driver;

int generic_ipi::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

int generic_ipi::sendIPI(size_t cpuID, IPI_MSG msg) {
	(void) cpuID;
	(void) msg;

	return -ENXIO;
}

int generic_ipi::registerHandler(IPI_MSG msg, lib::function<int()> handler) {
	(void) msg;
	(void) handler;

	return -ENXIO;
}
