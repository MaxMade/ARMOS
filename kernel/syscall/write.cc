#include <unistd.h>
#include <cerrno.h>
#include <ostream.h>
#include <kernel/syscall/write.h>
#include <kernel/syscall/syscall.h>

ssize_t syscall::write(int fd, const void* buf, size_t count) {
	/* Currently only stdout is supported */
	if (fd != STDOUT_FILENO)
		return EBADF;

	lib::ostream cout;

	/* Write character-wise */
	for (size_t i = 0; i < count; i++)
		cout << static_cast<const char*>(buf)[i];

	return count;
}

void syscall::__write(irq::ExceptionContext* irq) {
	/* Get values */
	auto fd = syscall::getSyscallArg<0, int>(irq);
	auto buf = syscall::getSyscallArg<1, const void*>(irq);
	auto count = syscall::getSyscallArg<2, size_t>(irq);

	/* Check permissions */
	auto readable = syscall::isReadable(buf, count);

	/* Perform actual write */
	ssize_t ret = EFAULT;
	if (readable)
		ret = write(fd, buf, count);

	/* Save return value */
	syscall::setSyscallRetValue(irq, ret);
}
