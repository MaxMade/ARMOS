#include <cstring.h>

void* memchr(const void* s, int c, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (reinterpret_cast<const unsigned char*>(s)[i] == static_cast<unsigned char>(c))
			return (void*) &((char*) s)[i];
	}

	return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
	for (size_t i = 0; i < n; i++) {
		char c1 = reinterpret_cast<const char*>(s1)[i];
		char c2 = reinterpret_cast<const char*>(s2)[i];

		if (c1 < c2)
			return -1;

		if (c2 > c1)
			return 1;
	}

	return 0;
}

void* memcpy(void* dest, const void* src, size_t n) {
	for (size_t i = 0; i < n; i++)
		reinterpret_cast<char*>(dest)[i] = reinterpret_cast<const char*>(src)[i];
	return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
	char* d = reinterpret_cast<char*>(dest);
	const char* s = reinterpret_cast<char const*>(src);

	if (s < d) {
		s += n;
		d += n;
		while (n--)
			*--d =* --s;
	} else {
		while (n--)
			*d++ =* s++;
	}

	return dest;
}

void* memset(void* dest, int c, size_t n) {
	for (size_t i = 0; i < n; i++)
		reinterpret_cast<char*>(dest)[i] = (char) c;

	return dest;
}

char* strcat(char* dest, const char* src) {
	size_t i = 0;
	size_t j = 0;
	char* d = reinterpret_cast<char*>(dest);
	const char* s = reinterpret_cast<const char*>(src);

	for (i = 0; d[i] != '\0'; i++);

	for (j = 0; s[j] != '\0'; j++)
		d[i+j] = s[j];

	d[i+j] = '\0';

	return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
	size_t i = 0;
	size_t j = 0;
	char* d = reinterpret_cast<char*>(dest);
	const char* s = reinterpret_cast<const char*>(src);

	for (i = 0; d[i] != '\0'; i++);

	for (j = 0; s[j] != '\0' && j < n; j++)
		d[i+j] = s[j];

	d[i+j] = '\0';

	return dest;
}

int strcmp(const char* s1, const char* s2) {
	while(*s1 && (*s1 ==* s2)) {
		s1++;
		s2++;
	}

	return* reinterpret_cast<const unsigned char*>(s1) -* reinterpret_cast<const unsigned char*>(s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
	while (n &&* s1 && (*s1 ==* s2)) {
		++s1;
		++s2;
		--n;
	}

	if (n == 0)
		return 0;

	return* reinterpret_cast<const unsigned char*>(s1) -* reinterpret_cast<const unsigned char*>(s2);
}

char* strcpy(char* dest, const char* src) {
	char* ret = dest;

	while ((*dest++ =* src++));

	return ret;
}

char* strncpy(char* dest, const char* src, size_t n) {
	char* ret = dest;

	while (n-- && (*dest++ =* src++));

	return ret;
}

size_t strlen(const char* s) {
	size_t ret = 0;

	while (s[ret] != '\0')
		ret++;

	return ret;
}

size_t strnlen(const char* s, size_t n) {
	size_t ret = 0;

	while (n-- && s[ret] != '\0')
		ret++;

	return ret;
}

char* strchr(const char* s, int c) {
	for (size_t i = 0; s[i] != '\0'; i++) {
		if (s[i] == c)
			return const_cast<char*>(&s[i]);
	}

	return nullptr;
}

static const char* error_strings[] = {
	"Operation not permitted",
	"No such file or directory",
	"No such process",
	"Interrupted system call",
	"Input/output error",
	"No such device or address",
	"Argument list too long",
	"Exec format error",
	"Bad file descriptor",
	"No child processes",
	"Resource temporarily unavailable",
	"Cannot allocate memory",
	"Permission denied",
	"Bad address",
	"Block device required",
	"Device or resource busy",
	"File exists",
	"Invalid cross-device link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"Too many open files in system",
	"Too many open files",
	"Inappropriate ioctl for device",
	"Text file busy",
	"File too large",
	"No space left on device",
	"Illegal seek",
	"Read-only file system",
	"Too many links",
	"Broken pipe",
	"Numerical argument out of domain",
	"Numerical result out of range",
	"Resource deadlock avoided",
	"File name too long",
	"No locks available",
	"Function not implemented",
	"Directory not empty",
	"",
	"Too many levels of symbolic links",
	"No message of desired type",
	"Identifier removed",
	"Channel number out of range",
	"Level 2 not synchronized",
	"Level 3 halted",
	"Level 3 reset",
	"Link number out of range",
	"Protocol driver not attached",
	"No CSI structure available",
	"Level 2 halted",
	"Invalid exchange",
	"Invalid request descriptor",
	"Exchange full",
	"No anode",
	"Invalid request code",
	"Invalid slot",
	"",
	"Bad font file format",
	"Device not a stream",
	"No data available",
	"Timer expired",
	"Out of streams resources",
	"Machine is not on the network",
	"Package not installed",
	"Object is remote",
	"Link has been severed",
	"Advertise error",
	"Srmount error",
	"Communication error on send",
	"Protocol error",
	"Multihop attempted",
	"RFS specific error",
	"Bad message",
	"Value too large for defined data type",
	"Name not unique on network",
	"File descriptor in bad state",
	"Remote address changed",
	"Can not access a needed shared library",
	"Accessing a corrupted shared library",
	".lib section in a.out corrupted",
	"Attempting to link in too many shared libraries",
	"Cannot exec a shared library directly",
	"Invalid or incomplete multibyte or wide character",
	"Interrupted system call should be restarted",
	"Streams pipe error",
	"Too many users",
	"Socket operation on non-socket",
	"Destination address required",
	"Message too long",
	"Protocol wrong type for socket",
	"Protocol not available",
	"Protocol not supported",
	"Socket type not supported",
	"Operation not supported",
	"Protocol family not supported",
	"Address family not supported by protocol",
	"Address already in use",
	"Cannot assign requested address",
	"Network is down",
	"Network is unreachable",
	"Network dropped connection on reset",
	"Software caused connection abort",
	"Connection reset by peer",
	"No buffer space available",
	"Transport endpoint is already connected",
	"Transport endpoint is not connected",
	"Cannot send after transport endpoint shutdown",
	"Too many references: cannot splice",
	"Connection timed out",
	"Connection refused",
	"Host is down",
	"No route to host",
	"Operation already in progress",
	"Operation now in progress",
	"Stale file handle",
	"Structure needs cleaning",
	"Not a XENIX named type file",
	"No XENIX semaphores available",
	"Is a named type file",
	"Remote I/O error",
	"Disk quota exceeded",
	"No medium found",
	"Wrong medium type",
	"Operation canceled",
	"Required key not available",
	"Key has expired",
	"Key has been revoked",
	"Key was rejected by service",
	"Owner died",
	"State not recoverable",
	"Operation not possible due to RF-kill",
	"Memory page has hardware error",
};

char* strerror(int errnum) {
	if (errnum <= 0 || errnum > 133 || errnum == 41 || errnum == 58)
		return nullptr;

	return const_cast<char*>(error_strings[errnum - 1]);
}
