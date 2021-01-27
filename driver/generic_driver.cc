#include <driver/generic_driver.h>

using namespace driver;

const char* generic_driver::getName() const {
	return name;
}

lib::pair<void*, size_t> generic_driver::getConfigSpace() const {
	return lib::pair(nullptr, 0);
}
