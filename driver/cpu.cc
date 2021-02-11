#include <cstring.h>
#include <driver/cpu.h>

using namespace driver;

void CPU::init(const char* name, size_t cores) {
	strncpy(this->name, name, nameLen);
	this->numCores = cores;
}

const char* CPU::getName() const {
	return name;
}

size_t CPU::getCoreCount() const {
	return numCores;
}
