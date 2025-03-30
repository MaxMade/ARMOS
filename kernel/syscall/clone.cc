#include <sched.h>
#include <unistd.h>
#include <cerrno.h>
#include <kernel/error.h>
#include <kernel/errno.h>
#include <kernel/syscall/clone.h>
#include <kernel/syscall/syscall.h>
#include <kernel/thread/scheduler.h>

int syscall::clone(int (*fn)(void *), void *stack, int flags, void* arg) {
	/* Thead must run in same VM */
	if ((flags & lib::CLONE_VM) == 0)
		return -EINVAL;

	/* Created thread must be in same thread group */
	if ((flags & lib::CLONE_THREAD) == 0)
		return -EINVAL;

	/* The remaining flags are unsupported */
	if ((flags & lib::CLONE_FS) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_FILES) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_SIGHAND) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_PIDFD) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_PTRACE) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_VFORK) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_PARENT) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWNS) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_SYSVSEM) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_SETTLS) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_PARENT_SETTID) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_CHILD_CLEARTID) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_CHILD_SETTID) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWCGROUP) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWUTS) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWIPC) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWUSER) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWPID) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_NEWNET) != 0)
		return -EINVAL;
	if ((flags & lib::CLONE_IO) != 0)
		return -EINVAL;

	return thread::scheduler.create((void* (*)(void*)) (void*) fn, stack, arg);
}

void syscall::__clone(irq::ExceptionContext* irq) {
	/* Get values */
	auto fn = syscall::getSyscallArg<0, int(*)(void*)>(irq);
	auto stack = syscall::getSyscallArg<1, void*>(irq);
	auto flag = syscall::getSyscallArg<2, int>(irq);
	auto arg = syscall::getSyscallArg<3, void*>(irq);


	/* Perform actual clone */
	auto ret = clone(fn, stack, flag, arg);

	/* Update errno if necessary */
	if (ret < 0) {
		auto curThread = thread::scheduler.getActive();
		__errnos[curThread->getID()] = -ret;
		ret = -1;
	}

	/* Save return value */
	syscall::setSyscallRetValue(irq, ret);
}
