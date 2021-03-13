#ifndef _INC_TYPE_TRAITS_H_
#define _INC_TYPE_TRAITS_H_

#include <cstddef.h>

/**
 * @file type_traits.h
 * @brief Type support
 */

namespace lib {

	/**
	 * @struct remove_reference
	 * @brief Remove reference
	 */
	template<typename T>
	struct remove_reference {
		typedef T type;
	};

	/**
	 * @struct remove_reference
	 * @brief Remove reference
	 */
	template<typename T>
	struct remove_reference<T&> {
		typedef T type;
	};

	/**
	 * @struct remove_reference
	 * @brief Remove reference
	 */
	template<typename T>
	struct remove_reference<T&&> {
		typedef T type;
	};

	/**
	 * @struct remove_cv
	 * @brief Remove const volatile qualifier
	 */
	template<typename T>
	struct remove_cv {
		typedef T type;
	};

	/**
	 * @struct remove_cv
	 * @brief Remove const volatile qualifier
	 */
	template<typename T>
	struct remove_cv<const T> {
		typedef T type;
	};

	/**
	 * @struct remove_cv
	 * @brief Remove const volatile qualifier
	 */
	template<typename T>
	struct remove_cv<volatile T> {
		typedef T type;
	};

	/**
	 * @struct remove_cv
	 * @brief Remove const volatile qualifier
	 */
	template<typename T>
	struct remove_cv<const volatile T> {
		typedef T type;
	};

	/**
	 * @struct remove_const
	 * @brief Remove const qualifier
	 */
	template<typename T>
	struct remove_const {
		typedef T type;
	};

	/**
	 * @struct remove_const
	 * @brief Remove const qualifier
	 */
	template<typename T>
	struct remove_const<const T> {
		typedef T type;
	};

	/**
	 * @struct remove_volatile
	 * @brief Remove volatile qualifier
	 */
	template<typename T>
	struct remove_volatile {
		typedef T type;
	};

	/**
	 * @struct remove_volatile
	 * @brief Remove volatile qualifier
	 */
	template<typename T>
	struct remove_volatile<volatile T> {
		typedef T type;
	};

	/**
	 * @struct remove_pointer
	 * @brief Remove pointer
	 */
	template<typename T>
	struct remove_pointer {
		typedef T type;
	};

	/**
	 * @struct remove_pointer
	 * @brief Remove pointer
	 */
	template<typename T>
	struct remove_pointer<T*> {
		typedef T type;
	};

	/**
	 * @struct remove_pointer
	 * @brief Remove pointer
	 */
	template<typename T>
	struct remove_pointer<T* const> {
		typedef T type;
	};

	/**
	 * @struct remove_pointer
	 * @brief Remove pointer
	 */
	template<typename T>
	struct remove_pointer<T* volatile> {
		typedef T type;
	};

	/**
	 * @struct remove_pointer
	 * @brief Remove pointer
	 */
	template<typename T>
	struct remove_pointer<T* const volatile> {
		typedef T type;
	};

	/**
	 * @struct integral_constant
	 * @brief Wrapper for static constant of specified type
	 */
	template<typename T, T v>
	struct integral_constant {
		/**
		 * @var value
		 * @brief Value of integral_constant
		 */
		static constexpr T value = v;

		/**
		 * @typedef value_type
		 * @brief Type of value
		 */
		using value_type = T;

		/**
		 * @typedef type
		 * @brief Type of integral_constant
		 */
		using type = integral_constant;

		/**
		 * @fn constexpr operator value_type() const
		 * @brief Returns wrapped value
		 */
		constexpr operator value_type() const {
			return value;
		}

		/**
		 * @fn constexpr value_type operator()() const
		 * @brief Returns wrapped value
		 */
		constexpr value_type operator()() const {
			return value;
		}
	};

	/**
	 * @typedef true_type
	 * @brief Typedef for true constant
	 */
	using true_type = integral_constant<bool, true>;

	/**
	 * @typedef false_type
	 * @brief Typedef for false constant
	 */
	using false_type = integral_constant<bool, false>;

	/**
	 * @struct is_same
	 * @brief Compile-Time Check of two types are the same
	 */
	template<typename T, typename U>
	struct is_same : false_type {};

	/**
	 * @struct is_same
	 * @brief Compile-Time Check of two types are the same
	 */
	template<typename T>
	struct is_same<T, T> : true_type {};

	/**
	 * @struct is_void
	 * @brief Is void
	 */
	template<typename T>
	struct is_void : is_same<void, typename remove_cv<T>::type> {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<typename T>
	struct is_integral_helper : false_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<bool> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<char> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<short> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<int> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<long> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<long long> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<unsigned char> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<unsigned short> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<unsigned int> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<unsigned long> : true_type {};

	/**
	 * @struct is_integral_helper
	 * @brief Helper for is_integral
	 */
	template<>
	struct is_integral_helper<unsigned long long> : true_type {};

	/**
	 * @struct is_integral
	 * @brief Check if integral type
	 */
	template<typename T>
	struct is_integral : is_integral_helper<typename remove_cv<T>::type> {};

	/**
	 * @struct is_floating_point_helper
	 * @brief Helper for is_floating_point
	 */
	template<typename T>
	struct is_floating_point_helper : false_type {};

	/**
	 * @struct is_floating_point_helper
	 * @brief Helper for is_floating_point
	 */
	template<>
	struct is_floating_point_helper<float> : true_type {};

	/**
	 * @struct is_floating_point_helper
	 * @brief Helper for is_floating_point
	 */
	template<>
	struct is_floating_point_helper<double> : true_type {};

	/**
	 * @struct is_floating_point_helper
	 * @brief Helper for is_floating_point
	 */
	template<>
	struct is_floating_point_helper<long double> : true_type {};

	/**
	 * @struct is_floating_point
	 * @brief Check if floating_point type
	 */
	template<typename T>
	struct is_floating_point : is_floating_point_helper<typename remove_cv<T>::type> {};

	/**
	 * @struct is_array
	 * @brief Chekc if array type
	 */
	template<typename T>
	struct is_array : false_type {};

	/**
	 * @struct is_array
	 * @brief Chekc if array type
	 */
	template<typename T>
	struct is_array<T[]> : true_type {};

	/**
	 * @struct is_array
	 * @brief Chekc if array type
	 */
	template<typename T, size_t N>
	struct is_array<T[N]> : true_type {};

	/**
	 * @struct is_arithmetic
	 * @brief Check if arithmetic type
	 */
	template<typename T>
	struct is_arithmetic : integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

	/**
	 * @struct is_pointer_helper
	 * @brief Helpfer for is_pointer
	 */
	template<typename T>
	struct is_pointer_helper : false_type {};

	/**
	 * @struct is_pointer_helper
	 * @brief Helpfer for is_pointer
	 */
	template<typename T>
	struct is_pointer_helper<T*> : true_type {};

	/**
	 * @struct is_pointer
	 * @brief Check if pointer
	 */
	template<typename T>
	struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

	/**
	 * @struct is_lvalue_reference
	 * @brief Check if lvalue reference
	 */
	template<typename T>
	struct is_lvalue_reference : false_type {};

	/**
	 * @struct is_lvalue_reference
	 * @brief Check if lvalue reference
	 */
	template<typename T>
	struct is_lvalue_reference<T&> : true_type {};

	/**
	 * @struct is_rvalue_reference
	 * @brief Check if rvalue reference
	 */
	template<typename T>
	struct is_rvalue_reference : false_type {};

	/**
	 * @struct is_rvalue_reference
	 * @brief Check if rvalue reference
	 */
	template<typename T>
	struct is_rvalue_reference<T&&> : true_type {};

	/**
	 * @struct is_const
	 * @brief Check if const value
	 */
	template<typename T>
	struct is_const : false_type {};

	/**
	 * @struct is_const
	 * @brief Check if const value
	 */
	template<typename T>
	struct is_const<const T> : true_type {};

	/**
	 * @struct is_volatile
	 * @brief Check if volatile value
	 */
	template<typename T>
	struct is_volatile : false_type {};

	/**
	 * @struct is_volatile
	 * @brief Check if volatile value
	 */
	template<typename T>
	struct is_volatile<volatile T> : true_type {};

	/**
	 * @struct is_reference
	 * @brief Check if refernece value
	 */
	template<typename T>
	struct is_reference : false_type {};

	/**
	 * @struct is_reference
	 * @brief Check if refernece value
	 */
	template<typename T>
	struct is_reference<T&> : true_type {};

	/**
	 * @struct is_reference
	 * @brief Check if refernece value
	 */
	template<typename T>
	struct is_reference<T&&> : true_type {};

	/**
	 * @struct is_member_pointer_helper
	 * @brief Helper for is_member_pointer
	 */
	template<typename T>
	struct is_member_pointer_helper : false_type {};

	/**
	 * @struct is_member_pointer_helper
	 * @brief Helper for is_member_pointer
	 */
	template<typename T, typename U>
	struct is_member_pointer_helper<T U::*> : true_type {};

	/**
	 * @struct is_member_pointer
	 * @brief Check if member pointer
	 */
	template<typename T>
	struct is_member_pointer : is_member_pointer_helper<typename remove_cv<T>::type> {};

	/**
	 * @struct is_signed_helper
	 * @brief Helper for is_signed
	 */
	template<typename T, bool = is_arithmetic<T>::value>
	struct is_signed_helper : integral_constant<bool, T(-1) < T(0)> {};

	/**
	 * @struct is_signed_helper
	 * @brief Helper for is_signed
	 */
	template<typename T>
	struct is_signed_helper <T, false> : false_type {};

	/**
	 * @struct is_signed
	 * @brief Check if signed value
	 */
	template<typename T>
	struct is_signed : is_signed_helper<T>::type {};

	/**
	 * @struct is_unsigned_helper
	 * @brief Helper for is_unsigned
	 */
	template<typename T, bool = is_arithmetic<T>::value>
	struct is_unsigned_helper : integral_constant<bool, T(0) < T(-1)> {};

	/**
	 * @struct is_unsigned_helper
	 * @brief Helper for is_unsigned
	 */
	template<typename T>
	struct is_unsigned_helper <T, false> : false_type {};

	/**
	 * @struct is_unsigned
	 * @brief Check if unsigned value
	 */
	template<typename T>
	struct is_unsigned : is_unsigned_helper<T>::type {};

	/**
	 * @struct remove_cvref
	 * @brief Remove const, volatile and reference
	 */
	template<typename T>
	struct remove_cvref {
		typedef remove_cv<remove_reference<T>> type;
	};

	/**
	 * @struct enable_if
	 * @brief Compile-Time enable (useful for function overloading with templates)
	 */
	template<bool B, class T = void>
	struct enable_if {};

	/**
	 * @struct enable_if
	 * @brief Compile-Time enable (useful for function overloading with templates)
	 */
	template<typename T>
	struct enable_if<true, T> {
		typedef T type;
	};

	/**
	 * @struct type_identity
	 * @brief Typedef for identity transformation
	 */
	template <typename T>
	struct type_identity {
		using type = T;
	};

	/**
	 * @fn auto add_lvalue_reference_Helper(int)
	 * @brief Helper for add_lvalue_reference
	 */
	template <typename T>
	auto add_lvalue_reference_Helper(int) -> type_identity<T&>;

	/**
	 * @fn auto add_lvalue_reference_Helper(...)
	 * @brief Helper for add_rvalue_reference
	 */
	template <typename T>
	auto add_lvalue_reference_Helper(...) -> type_identity<T>;

	/**
	 * @fn auto add_rvalue_reference_Helper(int)
	 * @brief Helper for add_rvalue_reference
	 */
	template <typename T>
	auto add_rvalue_reference_Helper(int) -> type_identity<T&&>;

	/**
	 * @fn auto add_rvalue_reference_Helper(...)
	 * @brief Helper for add_rvalue_reference
	 */
	template <typename T>
	auto add_rvalue_reference_Helper(...) -> type_identity<T>;

	/**
	 * @struct add_lvalue_reference
	 * @brief Create lvalue reference
	 */
	template <typename T>
	struct add_lvalue_reference : decltype(add_lvalue_reference_Helper<T>(0)) {};

	/**
	 * @struct add_rvalue_reference
	 * @brief Create rvalue reference
	 */
	template <typename T>
	struct add_rvalue_reference : decltype(add_rvalue_reference_Helper<T>(0)) {};

	/**
	 * @struct add_cs
	 * @brief Add const volatile modifier
	 */
	template <typename T>
	struct add_cv { typedef const volatile T type; };

	/**
	 * @struct add_const
	 * @brief Add const modifier
	 */
	template <typename T>
	struct add_const { typedef const T type; };

	/**
	 * @struct add_volatile
	 * @brief Add volatile modifier
	 */
	template <typename T>
	struct add_volatile { typedef volatile T type; };

} /* namespace lib */

#endif /* ifndef _INC_TYPE_TRAITS_H_ */
