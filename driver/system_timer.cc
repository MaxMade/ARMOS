#include <cerrno.h>
#include <kernel/math.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <driver/system_timer.h>

using namespace driver;

#define TICKS_PER_MS 1000

system_timer::system_timer() {
	name = ("brcm,bcm2835-system-timer");
}

int system_timer::init(const config& conf) {
	/* Set base */
	base = conf.getRange().first;

	/* Clear ticks */
	ticks = 0;

	/* Clear idxCallback */
	idxCallback = 0;

	/* Prepare interrupt configuration */
	intConfig.first = conf.getInterruptRange().first;
	intConfig.second = conf.getInterruptRange().second;

	/* Register interrupt controller */
	if (int err = intc.registerHandler(intConfig.first, intConfig.second, this); err)
		return err;

	return 0;
}

int system_timer::windup(size_t ms) {
	intv = ms;
	ts = readRegister<CLO>();
	ts += ms * TICKS_PER_MS;
	writeRegister<C1>(ts);
	return 0;
}

int system_timer::registerFunction(size_t ms, lib::function<int(void)> callback) {
	if (idxCallback == MAX_CALLBACKS)
		return -ENOMEM;

	size_t numTicks = math::roundUp(ms, intv) / intv;
	callbacks[idxCallback] = lib::pair(numTicks, lib::move(callback));
	if (!callbacks[idxCallback].second.isValid())
		return -ENOMEM;

	idxCallback++;
	return 0;
}

size_t system_timer::interval() const {
	return intv;
}

int system_timer::prologue() {
	/* Update timer ticks */
	this->ticks++;

	/* Windup timer (again) */
	this->ts += this->intv * TICKS_PER_MS;
	this->writeRegister<C1>(ts);
	this->writeRegister<CS>(1 << 1);

	return 1;
}

int system_timer::epilogue() {
	/* Call handlers */
	for (size_t i = 0; i < MAX_CALLBACKS; i++) {
		auto& callback = callbacks[i];
		if (ticks % callback.first == 0) {
			callback.second();
		}
	}

	/* Send IPIs to remaining cores */
	auto numCPUs = driver::cpus.numCPUs();
	for (size_t i = 1; i < numCPUs; i++) {
		driver::ipi.sendIPI(i, driver::IPI::IPI_MSG::RESCHEDULE);
	}

	return 0;
}
