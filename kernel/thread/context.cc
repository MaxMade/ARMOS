#include <cstdlib.h>
#include <cstring.h>
#include <kernel/math.h>
#include <kernel/config.h>
#include <kernel/thread/context.h>
#include <kernel/irq/exception_handler.h>

using namespace thread;

extern "C" void __context_switch(SavedContext* old, SavedContext* next);
extern "C" void restore_current_el_sp_el0_sync_entry();

Context::Context() : id(0), kernelStack(nullptr), userStack(nullptr), state(State::INVALID) { }

void Context::init(size_t id, void* kernelStack, void* userStack, bool kernel, void* retAddr) {
	this->id = id;
	this->kernelStack = kernelStack;
	this->userStack = userStack;

	auto ptr =  reinterpret_cast<uintptr_t>(kernelStack);
	ptr += STACK_SIZE;
	ptr -= 8;
	ptr -= sizeof(irq::ExceptionContext);
	ptr = math::roundDown(ptr, 8);
	auto* kickoff = reinterpret_cast<irq::ExceptionContext*>(ptr);
	memset(kickoff, 0, sizeof(*kickoff));

	/* Set kernel stack */
	this->savedContext.sp = reinterpret_cast<uintptr_t>(ptr);

	/* Set return address */
	this->savedContext.x30 = reinterpret_cast<uintptr_t>(restore_current_el_sp_el0_sync_entry);

	/* Set user stack */
	auto userPtr = reinterpret_cast<uintptr_t>(userStack);
	userPtr += STACK_SIZE;
	userPtr -= 8;
	userPtr = math::roundDown(userPtr, 8);
	kickoff->sp_el0 = userPtr;

	/* Set ELR */
	kickoff->elr_el1 = reinterpret_cast<uintptr_t>(retAddr);

	/* Set SPSR
	 * [3:1] Exception Level
	 * [0]   Stack Pointer select
	 */
	if (kernel) {
		kickoff->spsr_el1 = 0b0101;
	} else {
		kickoff->spsr_el1 = 0x0000;
	}

	state = State::CREATED;
}

Context::~Context() {
	lib::free(kernelStack);
	lib::free(userStack);
}


size_t Context::getID() const {
	return id;
}

void Context::setState(State state) {
	this->state = state;
}

State Context::getState() const {
	return state;
}

void Context::setExceptionContext(irq::ExceptionContext* exceptionContext) {
	this->exceptionContext = exceptionContext;
}

irq::ExceptionContext* Context::getExceptionContext() const {
	return exceptionContext;
}

void Context::switching(Context* old, Context* next) {
	__context_switch(&old->savedContext, &next->savedContext);
}
