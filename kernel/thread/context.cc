#include <cstdlib.h>
#include <cstring.h>
#include <kernel/math.h>
#include <kernel/error.h>
#include <kernel/config.h>
#include <kernel/mm/paging.h>
#include <kernel/lock/softirq.h>
#include <kernel/thread/context.h>
#include <kernel/thread/scheduler.h>
#include <kernel/irq/exception_handler.h>

using namespace thread;

extern "C" void restore_current_el_sp_el0_sync_entry();
extern "C" void __exit(int exit_value);
extern "C" void __context_switch(SavedContext* old, SavedContext* next);
extern "C" void __context_kickoff();

Context::Context() : id(0), kernelStack(nullptr), userStack(nullptr), state(State::INVALID) { }

int Context::init(size_t id, bool kernel, void* startAddr, void* arg, void* userStack) {
	this->kernelStack = reinterpret_cast<void*>(lib::malloc(STACK_SIZE));
	if (this->kernelStack == nullptr)
		return -ENOMEM;

	this->id = id;
	this->userStack = userStack;
	this->state = State::CREATED;

	/* Prepare kickoff pointer */
	auto ptr =  reinterpret_cast<uintptr_t>(kernelStack);
	ptr += STACK_SIZE;
	ptr -= CPU::getStackAlignment();
	ptr -= sizeof(irq::ExceptionContext);
	ptr = math::roundDown(ptr, CPU::getStackAlignment());
	auto* kickoff = reinterpret_cast<irq::ExceptionContext*>(ptr);
	lib::memset(kickoff, 0, sizeof(*kickoff));

	/* Set exit address */
	kickoff->x30 = reinterpret_cast<uint64_t>(__exit);

	/* Prepare argument */
	kickoff->x0 = reinterpret_cast<uint64_t>(arg);

	/* Prepare thread ID */
	kickoff->tpidr_el0 = id;


	/* If an user process is created, the __unlock_global_scheduler function
	 * must be called in order to release the lock of the global scheduler
	 * (which was acquired by scheduler.schedule() function call).  However, if
	 * this function is executed on the previously prepared kernel stack, it
	 * will corrupt its later register contents. Therefore, its SP will point
	 * below the kickoff variable and update its content afterwards. For this
	 * purpose, the __context_kickoff function exits.
	 * In case of a kernel thread, the restore_current_el_sp_el0_sync_entry can
	 * be called directly.
	 */
	if (kernel) {
		/* Set return address */
		this->savedContext.x30 = reinterpret_cast<uintptr_t>(restore_current_el_sp_el0_sync_entry);

		/* Set kernel stack */
		this->savedContext.sp = ptr;

	} else {
		/* x19 will be used to store the actual stack pointer */
		this->savedContext.x19 = reinterpret_cast<uintptr_t>(kickoff);

		/* Set return address */
		this->savedContext.x30 = reinterpret_cast<uintptr_t>(__context_kickoff);

		/* Set kernel stack */
		this->savedContext.sp = ptr - PAGESIZE;
	}

	/* Set user stack */
	auto userPtr = reinterpret_cast<uintptr_t>(userStack);
	userPtr += STACK_SIZE;
	userPtr -= CPU::getStackAlignment();
	userPtr = math::roundDown(userPtr, CPU::getStackAlignment());

	kickoff->sp_el0 = userPtr;

	/* Set ELR */
	kickoff->elr_el1 = reinterpret_cast<uintptr_t>(startAddr);

	/* Set SPSR
	 * [3:1] Exception Level
	 * [0]   Stack Pointer select
	 */
	if (kernel) {
		kickoff->spsr_el1 = 0b0101;
	} else {
		kickoff->spsr_el1 = 0x0000;
	}

	return 0;
}

Context::~Context() {
	lib::free(kernelStack);
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

bool Context::operator==(const Context& o) const {
	return id == o.id;
}

bool Context::operator!=(const Context& o) const {
	return id != o.id;
}

bool Context::operator<=(const Context& o) const {
	return id <= o.id;
}

bool Context::operator>=(const Context& o) const {
	return id >= o.id;
}

bool Context::operator<(const Context& o) const {
	return id < o.id;
}

bool Context::operator>(const Context& o) const {
	return id > o.id;
}

void Context::switching(Context* old, Context* next) {
	__context_switch(&old->savedContext, &next->savedContext);
}

extern "C" void __unlock_for_kickoff() {
	CPU::disableInterrupts();
	scheduler.__unlock();
	lock::softirq.__unlock();
}
