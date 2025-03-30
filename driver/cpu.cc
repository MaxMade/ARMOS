#include <cerrno.h>
#include <cstring.h>
#include <utility.h>
#include <driver/cpu.h>

using namespace driver;


CPU::CPU(const char* name, void* spintable) {
	strncpy(this->name, name, nameLen);
	this->spintable = spintable;
}

const char* CPU::getName() const {
	return name;
}

void* CPU::getSpintable() const {
	return spintable;
}

CPUs::CPUs() : idx(0) {}

int CPUs::registerCPU(CPU cpu) {
	if (idx == MAX_NUM_CPUS)
		return -ENOMEM;

	cpus[idx++] = lib::move(cpu);

	return 0;
}

size_t CPUs::numCPUs() const {
	return idx;
}

CPUs::iterator CPUs::begin() const {
	return &cpus[0];
}

CPUs::iterator CPUs::end() const {
	return &cpus[idx];
}
