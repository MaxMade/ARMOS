#include <kernel/irq/generic_sync_handler.h>

using namespace irq;

GenericSyncHandler::const_iterator GenericSyncHandler::beginEC() const {
	return &ecs[0];
}

GenericSyncHandler::const_iterator GenericSyncHandler::endEC() const {
	return &ecs[num_ecs];
}
