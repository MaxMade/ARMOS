#include <driver/config.h>

using namespace driver;

config::config(bool isValid) {
	addr = nullptr;
	size = 0;
	valid = isValid;
}

void config::setRange(void* addr, size_t size) {
	this->addr = addr;
	this->size = size;
}

lib::pair<void*, size_t> config::getRange() const {
	return lib::pair(addr, size);
}

bool config::isValid() const {
	return valid;
}

config::operator bool() const {
	return valid;
}
