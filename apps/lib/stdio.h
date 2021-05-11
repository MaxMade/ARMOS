#ifndef _INCLUDE_STDIO_H_
#define _INCLUDE_STDIO_H_

/**
 * @file apps/lib/stdio.h
 * @brief Standard buffered input/output
 */

#include <stdarg.h>
#include <stddef.h>

/**
 * @fn printf(const char *format, ...)
 * @brief Formatted output
 * @param format Format string
 * @warning Maximum ouput size is 512 character!
 * @return
 *
 *	- Number of written character
 */
int printf(const char *format, ...);

/**
 * @fn int sprintf(char *str, const char *format, ...)
 * @brief Write format string to string
 * @param str Destination string
 * @param format Format string
 * @return
 *
 *	- Number of written character
 */
int sprintf(char *str, const char *format, ...);

/**
 * @fn int snprintf(char *str, size_t size, const char *format, ...)
 * @brief Write format string to string (with upper bound)
 * @param str Destination string
 * @param size Size of string
 * @param format Format string
 * @return
 *
 *	- Number of written character
 */
int snprintf(char *str, size_t size, const char *format, ...);

/**
 * @fn int vprintf(const char *format, va_list ap)
 * @brief Formatted output via variable argument list
 * @param format Format string
 * @param ap Variable argument list
 * @warning Maximum ouput size is 512 character!
 * @return
 *
 *	- Number of written character
 */
int vprintf(const char *format, va_list ap);

/**
 * @fn int vsprintf(char *str, const char *format, ...)
 * @brief Write format string to string via variable argument list
 * @param str Destination string
 * @param format Format string
 * @return
 *
 *	- Number of written character
 */
int vsprintf(char *str, const char *format, va_list ap);

/**
 * @fn int vsnprintf(char *str, const size_t size, const char *format, va_list ap)
 * @brief Write format string to string via variable argument list (with upper bound)
 * @param str Destination string
 * @param size Size of format string
 * @param format Format string
 * @param ap Variable argument list
 * @return
 *
 *	- Number of written character
 */
int vsnprintf(char *str, const size_t size, const char *format, va_list ap);

/**
 * @fn void perror(const char* s)
 * @brief Print a system error message
 * @param s Massage
 */
void perror(const char* s);

#endif /* _INCLUDE_STDIO_H_ */
