#include <cerrno.h>
#include <driver/generic_driver.h>

using namespace driver;

size_t generic_driver::driverNum = 0;

generic_driver::generic_driver() {
	driverIdx = driverNum++;
}

const char* generic_driver::getName() const {
	return name;
}

size_t generic_driver::getNumDrivers() {
	return driverNum;
}

size_t generic_driver::getIndexDriver() const {
	return driverIdx;
}
