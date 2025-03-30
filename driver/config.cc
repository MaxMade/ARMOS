#include <driver/config.h>

using namespace driver;

config::config(bool isValid) {
	configSpace.first = nullptr;
	configSpace.second = 0;

	valid = isValid;
}

void config::setRange(void* addr, size_t size) {
	configSpace.first = addr;
	configSpace.second = size;
}

lib::pair<void*, size_t> config::getRange() const {
	return lib::pair(configSpace);
}

void config::setInterruptRange(void* addr, size_t size) {
	intSpace.first = addr;
	intSpace.second = size;
}

lib::pair<void*, size_t> config::getInterruptRange() const {
	return lib::pair(intSpace);
}

bool config::isValid() const {
	return valid;
}

config::operator bool() const {
	return valid;
}
