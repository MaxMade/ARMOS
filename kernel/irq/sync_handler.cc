#include <cerrno.h>
#include <cassert.h>
#include <hw/register/esr.h>
#include <kernel/irq/sync_handler.h>

using namespace irq;

int SyncHandler::registerHandler(GenericSyncHandler* handler) {
	/* Check if all slots are unused */
	for (auto it = handler->beginEC(); it != handler->endEC(); ++it) {
		if (handlers[*it] != nullptr)
			return -EINVAL;
	}

	/* Update slots */
	for (auto it = handler->beginEC(); it != handler->endEC(); ++it)
		handlers[*it] = handler;

	return 0;
}

driver::generic_driver* SyncHandler::getHandler() {
	hw::reg::ESR esr;
	auto idx = esr.getEC();
	assert(idx < NUM_HANDLERS);
	return handlers[idx];
}
