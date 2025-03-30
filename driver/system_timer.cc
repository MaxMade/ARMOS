#include <cerrno.h>
#include <kernel/cpu.h>
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
	ticks.store(0);

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
	if (::CPU::getProcessorID() != 0)
		return 0;

	intv = ms;
	ts = readRegister<CLO>();
	ts += ms * TICKS_PER_MS;
	writeRegister<C1>(ts);
	return 0;
}

int system_timer::registerFunction(size_t ms, lib::function<int(void)> callback) {
	lock.lock();
	if (idxCallback == MAX_CALLBACKS) {
		lock.unlock();
		return -ENOMEM;
	}

	size_t numTicks = math::roundUp(ms, intv) / intv;
	callbacks[idxCallback] = lib::pair(numTicks, lib::move(callback));
	if (!callbacks[idxCallback].second.isValid()) {
		lock.unlock();
		return -ENOMEM;
	}

	idxCallback++;
	lock.unlock();
	return 0;
}

size_t system_timer::interval() const {
	return intv;
}

/**
 * @fn size_t getTicks() const
 * @brief Get number of ticks since startup
 */
size_t system_timer::getTicks() const {
	return ticks.load();
}

int system_timer::prologue(irq::ExceptionContext* context) {
	(void) context;

	/* Update timer ticks */
	this->ticks.fetch_add(1);

	/* Windup timer (again) */
	this->ts += this->intv * TICKS_PER_MS;
	this->writeRegister<C1>(ts);
	this->writeRegister<CS>(1 << 1);

	return 1;
}

int system_timer::epilogue() {
	/* Call handlers */
	lock.lock();
	for (size_t i = 0; i < MAX_CALLBACKS; i++) {
		auto& callback = callbacks[i];
		if (ticks.load() % callback.first == 0) {
			callback.second();
		}
	}
	lock.unlock();

	/* Send IPIs to remaining cores */
	auto numCPUs = driver::cpus.numCPUs();
	for (size_t i = 1; i < numCPUs; i++) {
		driver::ipi.sendIPI(i, driver::IPI::IPI_MSG::RESCHEDULE);
	}

	return 0;
}
