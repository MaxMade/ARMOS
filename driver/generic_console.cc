#include <cerrno.h>
#include <driver/generic_console.h>

using namespace driver;

int generic_console::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

void generic_console::write(const char* buf, size_t len) {
	(void) buf;
	(void) len;
}

char generic_console::read() {
	return '\0';
}


lib::pair<void*, size_t> generic_console::getConfigSpace() const {
	return lib::pair(nullptr, 0);
}
