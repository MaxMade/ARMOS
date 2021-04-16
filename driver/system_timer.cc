#include <cerrno.h>
#include <kernel/cpu.h>
#include <kernel/math.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <driver/system_timer.h>
#include <kernel/thread/smp.h>
#include <kernel/thread/scheduler.h>

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

int system_timer::registerFunction(size_t ms, int (*callback)()) {
	lock.lock();
	if (idxCallback == MAX_CALLBACKS) {
		lock.unlock();
		return -ENOMEM;
	}

	size_t numTicks = math::roundUp(ms, intv) / intv;
	callbacks[idxCallback] = lib::pair(numTicks, callback);
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
	for (size_t i = 0; i < MAX_CALLBACKS; i++) {
		if (callbacks[i].second == nullptr)
			continue;

		if (ticks.load() % callbacks[i].first == 0) {
			callbacks[i].second();
		}
	}

	/* Send IPIs to remaining cores */
	auto numCPUs = thread::smp.getRegisteredCPUS();
	for (size_t i = 0; i < numCPUs; i++) {
		driver::ipi.sendIPI(1 + i, driver::IPI::IPI_MSG::RESCHEDULE);
	}

	/* Schedule */
	thread::scheduler.schedule();

	return 0;
}
