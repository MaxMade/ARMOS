#include <cerrno.h>
#include <cassert.h>
#include <sys/syscall.h>
#include <kernel/error.h>
#include <kernel/debug/panic.h>
#include <kernel/syscall/write.h>
#include <kernel/irq/syscall.h>
#include <kernel/irq/exception_handler.h>

using namespace irq;

SyscallHandler::SyscallHandler() {
	/* Register exception class */
	ecs = __ecs;
	num_ecs = 2;

	/* Register handlers */
	memset(&handlers, 0, sizeof(void*) * NUM_HANDLERS);
	handlers[SYS_WRITE] = syscall::__write;
}

int SyscallHandler::prologue(irq::ExceptionContext* context) {
	/* Syscall number */
	auto num = reinterpret_cast<size_t>(context->x8);

	/* Check for suitable handler */
	if (handlers[num] == nullptr)
		return -EINVAL;

	/* Save context for prologue */
	savedContext.get() = context;

	/* Signal required epilogue */
	return 1;
}

int SyscallHandler::epilogue() {
	/* Syscall number */
	auto num = reinterpret_cast<size_t>(savedContext.get()->x8);

	/* Execute handler */
	handlers[num](savedContext.get());

	return 0;
}
