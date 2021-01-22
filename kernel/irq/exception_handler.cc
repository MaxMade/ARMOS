#include <cstdint.h>
#include <hw/register/esr.h>
#include <kernel/irq/exception_handler.h>

extern "C" {

	void current_el_sp_el0_sync(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_el0_irq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_el0_fiq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_el0_serror(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_elx_sync(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_elx_irq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_elx_fiq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void current_el_sp_elx_serror(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch64_sync(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch64_irq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch64_fiq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch64_serror(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch32_sync(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch32_irq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch32_fiq(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}

	void lower_el_aarch32_serror(void* saved_state) {
		(void) saved_state;

		hw::reg::ESR esr;
		const char* description = esr.getECString();
		(void) description;

		/* TODO: Implement me */
	}
}

extern "C" uintptr_t exception_vector;

void* irq::getExceptionVector() {
	return &exception_vector;
}
