#include <driver/generic_console.h>

using namespace driver;

bool generic_console::init(const config& conf) {
	(void) conf;
	return false;
}

void generic_console::write(const char* buf, size_t len) {
	(void) buf;
	(void) len;
}

char generic_console::read() {
	return '\0';
}
