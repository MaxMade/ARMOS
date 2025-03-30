#ifndef _INC_UTILITY_H_
#define _INC_UTILITY_H_

#include <cstddef.h>
#include <type_traits.h>

/**
 * @file utility.h
 * @brief General utility libary
 */

namespace lib {

	/**
	 * @fn typename remove_reference<T>::type&& move(T&& t)
	 * @brief Indicate move semantic
	 */
	template<typename T>
	typename remove_reference<T>::type&& move(T&& t) {
		return static_cast<typename remove_reference<T>::type&&>(t);

	}

	/**
	 * @fn T&& forward(typename remove_reference<T>::type& t)
	 * @brief Forward reference
	 */
	template<typename T>
	T&& forward(typename remove_reference<T>::type& t) {
		return static_cast<T&&>(t);
	}

	/**
	 * @fn T&& forward(typename remove_reference<T>::type&& t)
	 * @brief Forward reference
	 */
	template<typename T>
	T&& forward(typename remove_reference<T>::type&& t) {
		return static_cast<T&&>(t);
	}

	/**
	 * @fn T exchange(T& obj, U&& new_value)
	 * @brief Exchange values
	 */
	template<typename T, typename U = T>
	T exchange(T& obj, U&& new_value) {
		T old_value = move(obj);
		obj = forward<U>(new_value);
		return old_value;
	}

	/**
	 * @fn void swap(T& a, T& b)
	 * @brief Swap values
	 */
	template<typename T>
	void swap(T& a, T& b) {
		T tmp(move(a));
		a = move(b);
		b = move(tmp);
	}

	/**
	 * @fn void swap(T (&a)[N], T (&b)[N])
	 * @brief Swap arrays
	 */
	template<typename T, size_t N>
	void swap(T (&a)[N], T (&b)[N]) {
		for (size_t i = 0; i < N; ++i) {
			swap(a[i], b[i]);
		}
	}

	/**
	 * @class pair
	 * @brief Pair of two arbitrary values
	 */
	template<typename T1, typename T2>
	class pair {
		public:
			/**
			 * @typedef first_type
			 * @brief Type of first entry
			 */
			using first_type = T1;

			/**
			 * @typedef second_type
			 * @brief Type of second entry
			 */
			using second_type = T2;

			/**
			 * @var first
			 * @brief First entry
			 */
			first_type first;

			/**
			 * @var second
			 * @brief Second entry
			 */
			second_type second;

			/**
			 * @fn pair()
			 * @brief Default constructor
			 */
			pair() {}

			/**
			 * @fn pair(const T1& x, const T2& y)
			 * @brief Constructor
			 */
			pair(const T1& x, const T2& y) : first(x), second(y) { }

			/**
			 * @fn pair(U1&& x, U2&& y)
			 * @brief Constructor
			 */
			template<typename U1, typename U2>
			pair(U1&& x, U2&& y) : first(forward<U1>(x)), second(forward<U2>(y)) { }

			/**
			 * @fn pair(const pair<U1, U2>& p)
			 * @brief Alias Copy Constructor
			 */
			template<typename U1, typename U2>
			pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }

			/**
			 * @fn pair(pair<U1, U2>&& p)
			 * @brief Alias Move Constructor
			 */
			template<typename U1, typename U2>
			pair(pair<U1, U2>&& p) : first(forward<U1>(p.first)), second(forward<U2>(p.second)) { }

			/**
			 * @fn pair(const pair& p)
			 * @brief Copy Constructor
			 */
			pair(const pair& p) = default;

			/**
			 * @fn pair(pair&& p)
			 * @brief Move Constructor
			 */
			pair(pair&& p) = default;

			/**
			 * @fn pair& operator=(const pair& other)
			 * @brief Copy Assignment
			 */
			pair& operator=(const pair& other) {
				this->first = other.first;
				this->second = other.second;
				return *this;
			}

			/**
			 * @fn pair& operator=(pair&& other)
			 * @brief Move Assignment
			 */
			pair& operator=(pair&& other) {
				this->first = move(other.first);
				this->second = move(other.second);
				return *this;
			}

			/**
			 * @fn pair& operator=(const pair<U1, U2>& p)
			 * @brief Alias Copy Assignment
			 */
			template<typename U1, typename U2>
			pair& operator=(const pair<U1, U2>& p) {
				this->first = p.first;
				this->second = p.second;
				return *this;
			}

			/**
			 * @fn pair& operator=(pair<U1, U2>&& p)
			 * @brief Alias Move Assignment
			 */
			template<typename U1, typename U2>
			pair& operator=(pair<U1, U2>&& p) {
				this->first = forard(p.first);
				this->second = forwar(p.second);
				return *this;
			}


			/**
			 * @fn void swap(pair& other)
			 * @brief Swap pairs
			 */
			void swap(pair& other) {
				lib::swap(this->first, other.first);
				lib::swap(this->second, other.second);
			}
	};

	template<typename T1, typename T2>
	pair(T1, T2) -> pair<T1, T2>;

	/**
	 * @fn void swap(pair<T1,T2>& x, pair<T1,T2>& y)
	 * @brief Swap pairs
	 */
	template<typename T1, typename T2 >
	void swap(pair<T1,T2>& x, pair<T1,T2>& y) {
		lib::swap(x.first, y.first);
		lib::swap(x.second, y.second);
	}

} /* namespace lib */

#endif /* endif _INC_UTILITY_H_ */
