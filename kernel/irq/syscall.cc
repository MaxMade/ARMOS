#include <cassert.h>
#include <cerrno.h>
#include <kernel/error.h>
#include <kernel/debug/panic.h>
#include <kernel/irq/syscall.h>
#include <kernel/irq/exception_handler.h>

using namespace irq;

SyscallHandler::SyscallHandler() {
	ecs = __ecs;
	num_ecs = 2;
	memset(&handlers, 0, sizeof(void*) * NUM_HANDLERS);
}

int SyscallHandler::prologue(irq::ExceptionContext* context) {
	/* Syscall number */
	auto num = reinterpret_cast<size_t>(context->x8);

	/* Check for suitable handler */
	if (handlers[num] == nullptr)
		return -EINVAL;

	/* Save context for prologue */
	savedContext.get() = context;

	return 0;
}

int SyscallHandler::epilogue() {
	/* Syscall number */
	auto num = reinterpret_cast<size_t>(savedContext.get()->x8);

	/* Execute handler */
	handlers[num](savedContext.get());

	return 0;
}
