#include <ostream.h>
#include <cstdint.h>
#include <driver/drivers.h>
#include <hw/register/esr.h>
#include <kernel/error.h>
#include <kernel/debug/panic.h>
#include <kernel/lock/softirq.h>
#include <kernel/irq/sync_handler.h>
#include <kernel/irq/exception_handler.h>

extern "C" {

	void current_el_sp_el0_sync(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_el0_sync must never be used!", saved_state);
	}

	void current_el_sp_el0_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_el0_irq must never be used!", saved_state);
	}

	void current_el_sp_el0_fiq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_el0_fiq must never be used!", saved_state);
	}

	void current_el_sp_el0_serror(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_el0_serror must never be used!", saved_state);
	}

	void current_el_sp_elx_sync(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_elx_sync must never be used!", saved_state);
	}

	void current_el_sp_elx_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* Implement me correctly */
		auto driver = driver::intc.getHandler();
		if (isError(driver))
			debug::panic::generateFromIRQ("current_el_sp_elx_irq: No suitable driver found!", saved_state);

		auto err = lock::softirq.execute(driver, saved_state);
		if (isError(err))
			debug::panic::generateFromIRQ("current_el_sp_elx_irq: Error during softirq!", saved_state);
	}

	void current_el_sp_elx_fiq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_elx_fiq must never be used!", saved_state);
	}

	void current_el_sp_elx_serror(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("current_el_sp_elx_serror must never be used!", saved_state);
	}

	void lower_el_aarch64_sync(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();

		auto driver = irq::syncHandler.getHandler();
		if (driver == nullptr) {
			lib::panic panic;
			panic << "lower_el_aarch64_sync: " << description << "\n\r";
			panic << "ESR:\n\r";
			panic.setf(lib::ostream::hex, lib::ostream::basefield);
			panic.setf(lib::ostream::showbase);
			panic << "  ISS: " << esr.getISS() << "\n\r";
			panic << "  IL:  " << esr.getIL() << "\n\r";
			panic << "  EC:  " << esr.getEC() << "\n\r";
			panic.unsetf(lib::ostream::hex);
			panic.unsetf(lib::ostream::showbase);

			debug::panic::generateFromIRQ("No Handler found!", saved_state);
		}

		auto err = lock::softirq.execute(driver, saved_state);
		if (isError(err))
			debug::panic::generateFromIRQ("lower_el_aarch64_sync: Error during softirq!", saved_state);
	}

	void lower_el_aarch64_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		auto driver = driver::intc.getHandler();
		if (isError(driver))
			debug::panic::generateFromIRQ("current_el_sp_elx_irq: No suitable driver found!", saved_state);

		auto err = lock::softirq.execute(driver, saved_state);
		if (isError(err))
			debug::panic::generateFromIRQ("current_el_sp_elx_irq: Error during softirq!", saved_state);
	}

	void lower_el_aarch64_fiq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch64_fiq must never be used!", saved_state);
	}

	void lower_el_aarch64_serror(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch64_serror must never be used!", saved_state);
	}

	void lower_el_aarch32_sync(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch32_sync must never be used!", saved_state);
	}

	void lower_el_aarch32_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch32_irq must never be used!", saved_state);
	}

	void lower_el_aarch32_fiq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch32_fiq must never be used!", saved_state);
	}

	void lower_el_aarch32_serror(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		debug::panic::generateFromIRQ("lower_el_aarch32_serror must never be used!", saved_state);
	}
}

extern "C" uintptr_t exception_vector;

void* irq::getExceptionVector() {
	return &exception_vector;
}
