#ifndef _INC_CASSERT_H_
#define _INC_CASSERT_H_

#include <cstring.h>
#include <kernel/debug/panic.h>

/**
 * @file cassert.h
 * @brief Runtime assertion
 */

/**
 * @def assert
 * @brief Abort if assertion is false
 * @warning If \c expr is evaluated to false, the OS will kernel panic
 */
#define assert(expr)                                           \
	do {                                                       \
		if (__builtin_expect(!(expr), 0)) {                    \
			char lineStr[32];                                  \
			char tmp[512];                                     \
			long offset = 0;                                   \
                                                               \
			const char msg[] = "Assertion failed:";            \
			const char* test = #expr;                          \
			const char* func = __func__;                       \
			const char* file = __FILE__;                       \
			unsigned int line = __LINE__;                      \
                                                               \
			for (int i = 0; line > 0; i++) {                   \
				lineStr[i] = '0' + (line % 10);                \
				lineStr[i + 1] = 0;                            \
				line = line / 10;                              \
			}                                                  \
                                                               \
			int len = lib::strlen(lineStr);                    \
			for (int j = 0; j < (len / 2); j++) {              \
				char tmp = lineStr[j];                         \
				lineStr[j] = lineStr[len - 1 - j];             \
				lineStr[len - 1 - j] = tmp;                    \
			}                                                  \
                                                               \
			lib::strncpy(&tmp[offset], msg, 512 - offset);     \
			offset += lib::strlen(msg);                        \
			lib::strncpy(&tmp[offset], " '", 512 - offset);    \
			offset += 2;                                       \
			lib::strncpy(&tmp[offset], test, 512 - offset);    \
			offset += lib::strlen(test);                       \
			lib::strncpy(&tmp[offset], "' (", 512 - offset);   \
			offset += 3;                                       \
			lib::strncpy(&tmp[offset], func, 512 - offset);    \
			offset += lib::strlen(func);                       \
			lib::strncpy(&tmp[offset], "@", 512 - offset);     \
			offset += 1;                                       \
			lib::strncpy(&tmp[offset], file, 512 - offset);    \
			offset += lib::strlen(file);                       \
			lib::strncpy(&tmp[offset], ":", 512 - offset);     \
			offset += 1;                                       \
			lib::strncpy(&tmp[offset], lineStr, 512 - offset); \
			offset += lib::strlen(lineStr);                    \
			lib::strncpy(&tmp[offset], ")", 512 - offset);     \
			offset += 1;                                       \
                                                               \
			debug::panic::generate(tmp);                       \
		}                                                      \
	} while(0)

#endif /* ifndef _INC_CASSERT_H_ */
