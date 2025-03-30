#ifndef _INC_KERNEL_ERROR_H_
#define _INC_KERNEL_ERROR_H_

#include <cstdint.h>
#include <type_traits.h>

/**
 * @file kernel/error.h
 * @brief Error abstraction
 */

/**
 * @fn bool isError(T t)
 * @brief Check if t is error (negative) value
 */
template<typename T>
bool isError(T t) {
	static_assert(lib::is_pointer<T>() || lib::is_integral<T>(), "T should be pointer or integral value");

	intptr_t val = (intptr_t) t;
	if (val < 0)
		return true;

	return false;
}

/**
 * @fn T makeError(int errno)
 * @brief Make error for the given errno
 *
 */
template<typename T>
T makeError(int errno) {
	static_assert(lib::is_pointer<T>() || lib::is_integral<T>(), "T should be pointer or integral value");

	intptr_t ret = (intptr_t) (errno > 0 ? -errno : errno);
	return (T) ret;
}

/**
 * @fn T castError(U error)
 * @brief Cast error from type U to T
 */
template<typename T, typename U>
T castError(U error) {
	return (T) ((intptr_t) error);
}

#endif /* ifndef _INC_KERNEL_ERROR_H_ */
