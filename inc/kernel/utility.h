#ifndef _INC_KERNEL_UTILITY_H_
#define _INC_KERNEL_UTILITY_H_

#include <cstdint.h>
#include <kernel/cpu.h>
#include <kernel/irq/exception_handler.h>

/**
 * @file kernel/utility.h
 * @brief Kernel utilities
 */

namespace util {

	/**
	 * @fn constexpr T swapEndianness(T t)
	 * @brief Swap endianness of numeric values
	 * @tparam Type of numeric value
	 */
	template <typename T>
	constexpr T swapEndianness(T t) {
		static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8 || sizeof(T) == 16, "sizeof(T) should be 1, 2, 4, 8 or 16");

		if constexpr (sizeof(T) == 1)
			return t;

		if constexpr (sizeof(T) == 2)
			return __builtin_bswap16(t);

		if constexpr (sizeof(T) == 4)
			return __builtin_bswap32(t);

		if constexpr (sizeof(T) == 8)
			return __builtin_bswap64(t);

		if constexpr (sizeof(T) == 16)
			return __builtin_bswap128(t);

		return 0;
	}

	/**
	 * @fn T hostToBigEndian(T t)
	 * @brief Convert from host endianness to big endianness
	 * @tparam Type of numeric value
	 */
	template <typename T>
	T hostToBigEndian(T t) {
		if (CPU::isBigEndian())
			return t;

		return swapEndianness(t);
	}

	/**
	 * @fn T hostToLittleEndian(T t)
	 * @brief Convert from host endianness to little endianness
	 * @tparam Type of numeric value
	 */
	template <typename T>
	T hostToLittleEndian(T t) {
		if (CPU::isLittleEndian())
			return t;

		return swapEndianness(t);
	}

	/**
	 * @fn T littleEndianToHost(T t)
	 * @brief Convert from little endianness to host endianness
	 * @tparam Type of numeric value
	 */
	template <typename T>
	T littleEndianToHost(T t) {
		if (CPU::isLittleEndian())
			return t;

		return swapEndianness(t);
	}

	/**
	 * @fn T bigEndianToHost(T t)
	 * @brief Convert from big endianness to host endianness
	 * @tparam Type of numeric value
	 */
	template <typename T>
	T bigEndianToHost(T t) {
		if (CPU::isBigEndian())
			return t;

		return swapEndianness(t);
	}


	/**
	 * @fn void mmioWrite(T* ptr, T value)
	 * @brief Memory-Mapped Write Operation
	 */
	template <typename T>
	void mmioWrite(T* ptr, T value) {
		*static_cast<volatile T*>(ptr) = value;
	}

	/**
	 * @fn T mmioRead(T* ptr)
	 * @brief Memory-Mapped Read Operation
	 */
	template <typename T>
	T mmioRead(T* ptr) {
		return *static_cast<volatile T*>(ptr);
	}

	/**
	 * @fn int popcount(T x)
	 * @brief Number of 1 bits
	 */
	template<typename T>
	int popcount(T x) {
		int ret = 0;
		for (size_t i = 0; i < sizeof(x) * 8; i++) {
			ret += x & (1L << i);
		}
		return ret;
	}

	/**
	 * @fn int ffs(T x)
	 * @brief Find first set (or -1 in case of zero)
	 */
	template<typename T>
	int ffs(T x) {
		for (size_t i = 0; i < sizeof(x) * 8; i++) {
			 if (x & (1L << i))
				 return i;
		}

		return -1;
	}

	/**
	 * @fn int fls(T x)
	 * @brief Find last set (or -1 in case of zero)
	 */
	template<typename T>
	int fls(T x) {
		for (int i = sizeof(x) * 8 - 1; i >= 0; i--) {
			 if (x & (1L << i))
				 return i;
		}

		return -1;
	}

	/**
	 * @fn T getSyscallArg(irq::ExceptionContext* context)
	 * @brief Get N-th syscall argument
	 * @warning N must be in range [0, 5]
	 * @brief context Saved syscall context
	 */
	template<size_t N, typename T>
	T getSyscallArg(irq::ExceptionContext* context) {
		static_assert(N < 6, "Invalid argument number");

		if constexpr(N == 0)
			return (T) (uintptr_t) context->x0;

		if constexpr(N == 1)
			return (T) (uintptr_t) context->x1;

		if constexpr(N == 2)
			return (T) (uintptr_t) context->x2;

		if constexpr(N == 3)
			return (T) (uintptr_t) context->x3;

		if constexpr(N == 4)
			return (T) (uintptr_t) context->x4;

		if constexpr(N == 5)
			return (T) (uintptr_t) context->x5;
	}

	/**
	 * @fn void setSyscallRetValue(irq::ExceptionContext* context, T value)
	 * @brief Set syscall return value
	 */
	template<typename T>
	void setSyscallRetValue(irq::ExceptionContext* context, T value) {
		context->x0 = (uintptr_t) value;
	}

} /* namespace util */

#endif /* ifndef _INC_KERNEL_UTILITY_H_ */
