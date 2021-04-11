#ifndef _INC_APPS_LIB_STDARG_H_
#define _INC_APPS_LIB_STDARG_H_

/**
 * @file apps/lib/stdarg.h
 * @brief Handle variable argument list
 */

/**
 * @def va_list
 * @brief Variable argument list
 */
#define va_list __builtin_va_list

/**
 * @def va_start(ap, last)
 * @brief Initialize variable argument list
 * @param ap Argument lists
 * @param last Last argument before variable argument list
 */
#define va_start(ap, last)  __builtin_va_start(ap, last)

/**
 * @def va_copy(dest, src)
 * @brief Copy variable argument list
 * @param dest Destination
 * @param src Source
 */
#define va_copy(dest, src)  __builtin_va_copy(dest, src)

/**
 * @def va_arg(ap, type)
 * @brief Get next argument from argument list
 * @param ap Argument list
 * @param type Type of the next argument
 */
#define va_arg(ap, type)  __builtin_va_arg(ap, type)

/**
 * @def va_end(ap)
 * @brief Destroy variable argument list
 * @param ap Argument list
 */
#define va_end(ap)  __builtin_va_end(ap)

#endif /* ifndef _INC_APPS_LIB_STDARG_H_ */
