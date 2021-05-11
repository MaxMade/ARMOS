#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

/* DEFINES ----------------------------------------------------------------- */

#define FLAGS_ZEROPAD   (1 << 0)
#define FLAGS_LEFT      (1 << 1)
#define FLAGS_PLUS      (1 << 2)
#define FLAGS_SPACE     (1 << 3)
#define FLAGS_HASH      (1 << 4)
#define FLAGS_UPPERCASE (1 << 5)
#define FLAGS_CHAR      (1 << 6)
#define FLAGS_SHORT     (1 << 7)
#define FLAGS_LONG      (1 << 8)
#define FLAGS_LONG_LONG (1 << 9)
#define FLAGS_PRECISION (1 << 10)
#define FLAGS_ADAPT_EXP (1 << 11)
#define PRINTF_NTOA_BUFFER_SIZE 32

/* MACROS ------------------------------------------------------------------ */

#define IS_DIGIT(ch) ((ch >= '0') && (ch <= '9'))

/* HELPER ------------------------------------------------------------------ */

static void out_buffer(char character, void *buffer, size_t idx, size_t maxlen) {
	char *buf = (char *) buffer;
	if (idx < maxlen) {
		buf[idx] = character;
	}
}

static size_t out_rev_buffer(char *buffer, size_t idx, size_t maxlen, const char *buf,
		size_t len, unsigned int width, unsigned int flags) {

	const size_t start_idx = idx;

	if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
		for (size_t i = len; i < width; i++) {
			out_buffer(' ', buffer, idx++, maxlen);
		}
	}

	while (len) {
		out_buffer(buf[--len], buffer, idx++, maxlen);
	}

	if (flags & FLAGS_LEFT) {
		while (idx - start_idx < width) {
			out_buffer(' ', buffer, idx++, maxlen);
		}
	}

	return idx;
}

static void out_null(char character, void *buffer, size_t idx,
		size_t maxlen) {

	(void) character;
	(void) buffer;
	(void) idx;
	(void) maxlen;
}

static size_t ntoa_format(char *buffer, size_t idx, size_t maxlen, char *buf,
		size_t len, bool negative, unsigned int base,
		unsigned int prec, unsigned int width,
		unsigned int flags) {

	if (!(flags & FLAGS_LEFT)) {
		if (width && (flags & FLAGS_ZEROPAD) &&
				(negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
		while ((flags & FLAGS_ZEROPAD) && (len < width) &&
				(len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}

	if (flags & FLAGS_HASH) {
		if (!(flags & FLAGS_PRECISION) && len &&
				((len == prec) || (len == width))) {
			len--;
			if (len && (base == 16)) {
				len--;
			}
		}
		if ((base == 16) && !(flags & FLAGS_UPPERCASE) &&
				(len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'x';
		} else if ((base == 16) && (flags & FLAGS_UPPERCASE) &&
				(len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'X';
		} else if ((base == 2) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'b';
		}
		if (len < PRINTF_NTOA_BUFFER_SIZE) {
			buf[len++] = '0';
		}
	}

	if (len < PRINTF_NTOA_BUFFER_SIZE) {
		if (negative) {
			buf[len++] = '-';
		} else if (flags & FLAGS_PLUS) {
			buf[len++] = '+';
		} else if (flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}

	return out_rev_buffer(buffer, idx, maxlen, buf, len, width, flags);
}

static size_t ntoa_long(char *buffer, size_t idx, size_t maxlen,
		unsigned long value, bool negative, unsigned long base,
		unsigned int prec, unsigned int width,
		unsigned int flags) {

	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0;

	if (!value) {
		flags &= ~FLAGS_HASH;
	}

	if (!(flags & FLAGS_PRECISION) || value) {
		do {
			const char digit = (char) (value % base);
			buf[len++] = digit < 10
				? '0' + digit
				: (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}

	return ntoa_format(buffer, idx, maxlen, buf, len, negative,
			(unsigned int) base, prec, width, flags);
}

static size_t ntoa_long_long(char *buffer, size_t idx, size_t maxlen,
		unsigned long long value, bool negative,
		unsigned long long base, unsigned int prec,
		unsigned int width, unsigned int flags) {

	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0;

	if (!value) {
		flags &= ~FLAGS_HASH;
	}

	if (!(flags & FLAGS_PRECISION) || value) {
		do {
			const char digit = (char) (value % base);
			buf[len++] = digit < 10
				? '0' + digit
				: (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}

	return ntoa_format(buffer, idx, maxlen, buf, len, negative,
			(unsigned int) base, prec, width, flags);
}

static unsigned int atoi(const char** str)
{
  unsigned int i = 0U;
  while (IS_DIGIT(**str)) {
    i = i * 10U + (unsigned int)(*((*str)++) - '0');
  }
  return i;
}

int vsnprintf(char *buffer, const size_t size, const char *format, va_list va) {
	void (*out)(char, void *, size_t, size_t) = out_buffer;
	unsigned int flags, width, precision, n;
	size_t idx = 0;

	if (!buffer) {
		out = out_null;
	}

	while (*format) {
		if (*format != '%') {
			out(*format, buffer, idx++, size);
			format++;
			continue;
		} else {
			format++;
		}

		flags = 0;
		do {
			switch (*format) {
				case '0':
					flags |= FLAGS_ZEROPAD;
					format++;
					n = 1;
					break;

				case '-':
					flags |= FLAGS_LEFT;
					format++;
					n = 1;
					break;

				case '+':
					flags |= FLAGS_PLUS;
					format++;
					n = 1;
					break;

				case ' ':
					flags |= FLAGS_SPACE;
					format++;
					n = 1;
					break;

				case '#':
					flags |= FLAGS_HASH;
					format++;
					n = 1;
					break;

				default:
					n = 0;
					break;
			}
		} while (n);

		width = 0;
		if (IS_DIGIT(*format)) {
			width = atoi(&format);

		} else if (*format == '*') {
			const int w = va_arg(va, int);
			if (w < 0) {
				flags |= FLAGS_LEFT;
				width = (unsigned int) -w;

			} else {
				width = (unsigned int) w;
			}

			format++;
		}

		precision = 0;
		if (*format == '.') {
			flags |= FLAGS_PRECISION;
			format++;
			if (IS_DIGIT(*format)) {
				precision = atoi(&format);

			} else if (*format == '*') {
				const int prec = (int) va_arg(va, int);
				precision = prec > 0 ? (unsigned int) prec : 0;
				format++;
			}
		}

		switch (*format) {
			case 'l':
				flags |= FLAGS_LONG;
				format++;
				if (*format == 'l') {
					flags |= FLAGS_LONG_LONG;
					format++;
				}
				break;

			case 'h':
				flags |= FLAGS_SHORT;
				format++;
				if (*format == 'h') {
					flags |= FLAGS_CHAR;
					format++;
				}
				break;

			case 'j':
				flags |= FLAGS_LONG;
				format++;
				break;

			case 'z':
				flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;

			default:
				break;
		}

		switch (*format) {
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
			case 'b': {
						  unsigned int base;
						  if (*format == 'x' || *format == 'X') {
							  base = 16;
						  } else if (*format == 'o') {
							  base = 8;
						  } else if (*format == 'b') {
							  base = 2;
						  } else {
							  base = 10;
							  flags &= ~FLAGS_HASH;
						  }
						  if (*format == 'X') {
							  flags |= FLAGS_UPPERCASE;
						  }

						  if ((*format != 'i') && (*format != 'd')) {
							  flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
						  }

						  if (flags & FLAGS_PRECISION) {
							  flags &= ~FLAGS_ZEROPAD;
						  }

						  if ((*format == 'i') || (*format == 'd')) {
							  if (flags & FLAGS_LONG_LONG) {
								  const long long value = va_arg(va, long long);
								  idx = ntoa_long_long(
										  buffer, idx, size,
										  (unsigned long long) (value > 0 ? value : 0 - value), value < 0,
										  base, precision, width, flags);
							  } else if (flags & FLAGS_LONG) {
								  const long value = va_arg(va, long);
								  idx = ntoa_long(buffer, idx, size,
										  (unsigned long) (value > 0 ? value : 0 - value),
										  value < 0, base, precision, width, flags);
							  } else {
								  const int value = (flags & FLAGS_CHAR)
									  ? (char) va_arg(va, int)
									  : (flags & FLAGS_SHORT)
									  ? (short int) va_arg(va, int)
									  : va_arg(va, int);
								  idx = ntoa_long(buffer, idx, size,
										  (unsigned int) (value > 0 ? value : 0 - value),
										  value < 0, base, precision, width, flags);
							  }
						  } else {
							  if (flags & FLAGS_LONG_LONG) {
								  idx = ntoa_long_long(buffer, idx, size,
										  va_arg(va, unsigned long long), false, base,
										  precision, width, flags);
							  } else if (flags & FLAGS_LONG) {
								  idx = ntoa_long(buffer, idx, size, va_arg(va, unsigned long),
										  false, base, precision, width, flags);
							  } else {
								  const unsigned int value =
									  (flags & FLAGS_CHAR)
									  ? (unsigned char) va_arg(va, unsigned int)
									  : (flags & FLAGS_SHORT)
									  ? (unsigned short int) va_arg(va, unsigned int)
									  : va_arg(va, unsigned int);
								  idx = ntoa_long(buffer, idx, size, value, false, base, precision,
										  width, flags);
							  }
						  }
						  format++;
						  break;
					  }
			case 'c': {
						  unsigned int l = 1;
						  if (!(flags & FLAGS_LEFT)) {
							  while (l++ < width) {
								  out(' ', buffer, idx++, size);
							  }
						  }
						  out((char)va_arg(va, int), buffer, idx++, size);
						  if (flags & FLAGS_LEFT) {
							  while (l++ < width) {
								  out(' ', buffer, idx++, size);
							  }
						  }
						  format++;
						  break;
					  }

			case 's': {
						  const char *p = va_arg(va, char *);
						  unsigned int l = strnlen(p, precision ? precision : (size_t) - 1);
						  if (flags & FLAGS_PRECISION) {
							  l = (l < precision ? l : precision);
						  }
						  if (!(flags & FLAGS_LEFT)) {
							  while (l++ < width) {
								  out(' ', buffer, idx++, size);
							  }
						  }
						  while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
							  out(*(p++), buffer, idx++, size);
						  }
						  if (flags & FLAGS_LEFT) {
							  while (l++ < width) {
								  out(' ', buffer, idx++, size);
							  }
						  }
						  format++;
						  break;
					  }

			case 'p': {
						  out('0', buffer, idx++, size);
						  out('x', buffer, idx++, size);
						  width = sizeof(void *) * 2;
						  flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
						  const bool is_ll = (sizeof(uintptr_t) == sizeof(long long));
						  if (is_ll) {
							  idx =
								  ntoa_long_long(buffer, idx, size, (uintptr_t) va_arg(va, void *),
										  false, 16, precision, width, flags);
						  } else {
							  idx = ntoa_long(buffer, idx, size,
									  (unsigned long) ((uintptr_t) va_arg(va, void *)), false,
									  16, precision, width, flags);
						  }
						  format++;
						  break;
					  }

			case '%':
					  out('%', buffer, idx++, size);
					  format++;
					  break;

			default:
					  out(*format, buffer, idx++, size);
					  format++;
					  break;
		}
	}

	out('\0', buffer, idx < size ? idx : size - 1, size);

	return (int) idx;
}

int vsprintf(char *str, const char *format, va_list ap) {
	return vsnprintf(str, -1, format, ap);
}

int vprintf(const char *format, va_list ap) {
	char buf[512];
	vsprintf(buf, format, ap);
	return write(STDOUT_FILENO, buf, strnlen(buf, 512));
}

int snprintf(char *str, size_t size, const char *format, ...) {
	int ret = 0;
	va_list va;
	va_start(va, format);
	ret = vsnprintf(str, size, format, va);
	va_end(va);
	return ret;
}

int sprintf(char *str, const char *format, ...) {
	int ret = 0;
	va_list va;
	va_start(va, format);
	ret = vsnprintf(str, -1, format, va);
	va_end(va);
	return ret;
}

int printf(const char *format, ...) {
	int ret = 0;
	va_list va;
	va_start(va, format);
	ret = vprintf(format, va);
	va_end(va);
	return ret;
}

void perror(const char* s) {
	char buf[512];
	snprintf(buf, 512, "%s: %s\n", s, strerror(errno));
	printf("%s", buf);
}
