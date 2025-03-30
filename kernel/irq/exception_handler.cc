#include <cstdint.h>
#include <driver/drivers.h>
#include <hw/register/esr.h>
#include <kernel/irq/exception_handler.h>
#include <kernel/debug/panic.h>

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

		/* TODO: Implement me */
		debug::panic::generateFromIRQ("TODO: current_el_sp_elx_sync currently unimplemented!", saved_state);
	}

	void current_el_sp_elx_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me correctly */
		int ret = driver::intc.handleIRQ();
		(void) ret;
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
		(void) description;

		/* TODO: Implement me */
		debug::panic::generateFromIRQ("TODO: lower_el_aarch64_sync currently unimplemented!", saved_state);
	}

	void lower_el_aarch64_irq(irq::ExceptionContext* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me correcly */
		int ret = driver::intc.handleIRQ();
		(void) ret;
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
