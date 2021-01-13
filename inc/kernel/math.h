#ifndef _INC_KERNEL_MATH_H_
#define _INC_KERNEL_MATH_H_

#include<cstddef.h>
#include<type_traits.h>
#include<kernel/utility.h>

/**
 * @file kernel/math.h
 * @brief Common math operations
 */

namespace math {

	/**
	 * @fn T roundDown(T val, size_t alignment)
	 * @brief Round down to alignment
	 */
	template<typename T>
	T roundDown(T val, size_t alignment) {
		static_assert(!lib::is_same<lib::is_integral<T>, lib::true_type>(), "T must be a inegral value");
		return val - (val % alignment);
	}

	/**
	 * @fn T roundUp(T val, size_t alignment)
	 * @brief Round up to alignment
	 */
	template<typename T>
	T roundUp(T val, size_t alignment) {
		static_assert(!lib::is_same<lib::is_integral<T>, lib::true_type>(), "T must be a inegral value");

		T tmp = val + alignment - 1;
		return tmp - (tmp % alignment);
	}

	/**
	 * @fn bool isPowerOfTwo(T val)
	 * @brief Check if val is power of two
	 */
	template<typename T>
	bool isPowerOfTwo(T val) {
		static_assert(!lib::is_same<lib::is_integral<T>, lib::true_type>(), "T must be a inegral value");

		return util::popcount(val) == 1;
	}

	/**
	 * @fn int ld(T val)
	 * @brief Calculate logarithm dualis if given value
	 */
	template<typename T>
	int ld(T val) {
		static_assert(!lib::is_same<lib::is_integral<T>, lib::true_type>(), "T must be a inegral value");

		return util::fls(val);
	}

} /* namespace math */

#endif /* _INC_KERNEL_MATH_H_ */
