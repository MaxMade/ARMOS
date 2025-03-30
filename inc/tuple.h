#ifndef _INC_TUPLE_H_
#define _INC_TUPLE_H_

#include <cstddef.h>
#include <utility.h>

/**
 * @file tuple.h
 * @brief Tuple class and helpers
 */

namespace lib {

	/**
	 * @struct tuple
	 * @brief Tuple Container
	 */
	template <typename T, typename... Ts>
	struct tuple {
		/**
		 * @var value
		 * @brief Head value
		 */
		T value;

		/**
		 * @var rest
		 * @brief Tail values
		 */
		tuple<Ts...> rest;

		/**
		 * @fn tuple(const T& t, const Ts&... ts)
		 * @brief Construct tuple
		 */
		tuple(const T& t, const Ts&... ts) : value(t), rest(ts...) {}

		/**
		 * @fn void swap(tuple& o)
		 * @brief Swap tuples
		 */
		void swap(tuple& o) {
			lib::swap(*this, o);
		}
	};

	template <typename T>
	struct tuple<T> {
		/**
		 * @var value
		 * @brief Head value
		 */
		T value;

		/**
		 * @fn tuple(const T& t, const Ts&... ts)
		 * @brief Construct tuple
		 */
		tuple(const T& t) : value(t) {}

		/**
		 * @fn void swap(tuple& o)
		 * @brief Swap tuples
		 */
		void swap(tuple& o) {
			lib::swap(*this, o);
		}
	};

	//! @cond Doxygen_Suppress
	template <size_t i, typename T, typename... Ts>
	struct __GetHelper : __GetHelper<i-1, Ts...> {
		static_assert(i < sizeof...(Ts) + 1, "Index out of bounds");
	};

	template <typename T, typename... Ts>
	struct __GetHelper<0, T, Ts...> {
		T value;
	};

	template <size_t i>
	struct __Get {
		template <typename... Ts>
		static auto& get(tuple<Ts...>& t) {
			return __Get<i-1>::get(t.rest);
		}

		template <typename... Ts>
		static const auto& get(const tuple<Ts...>& t) {
			return __Get<i-1>::get(t.rest);
		}

		template <typename... Ts>
		static auto&& get(tuple<Ts...>&& t) {
			return __Get<i-1>::get(t.rest);
		}

		template <typename... Ts>
		static const auto&& get(const tuple<Ts...>&& t) {
			return __Get<i-1>::get(t.rest);
		}
	};

	template <>
	struct __Get<0> {
		template <typename T, typename... Ts>
		static T& get(tuple<T, Ts...>& t) {
			return t.value;
		}

		template <typename T, typename... Ts>
		static const T& get(const tuple<T, Ts...>& t) {
			return t.value;
		}

		template <typename T, typename... Ts>
		static T&& get(tuple<T, Ts...>&& t) {
			return t.value;
		}

		template <typename T, typename... Ts>
		static const T&& get(const tuple<T, Ts...>&& t) {
			return t.value;
		}
	};
	//! @endcond

	/**
	 * @fn auto& get(tuple<Ts...>& t)
	 * @brief Get i-th element
	 */
	template <size_t i, typename... Ts>
	auto& get(tuple<Ts...>& t) {
		return __Get<i>::get(t);
	}

	/**
	 * @fn const auto& get(const tuple<Ts...>& t)
	 * @brief Get i-th element
	 */
	template <size_t i, typename... Ts>
	const auto& get(const tuple<Ts...>& t) {
		return __Get<i>::get(t);
	}

	/**
	 * @fn auto&& get(tuple<Ts...>&& t)
	 * @brief Get i-th element
	 */
	template <size_t i, typename... Ts>
	auto&& get(tuple<Ts...>&& t) {
		return __Get<i>::get(t);
	}

	/**
	 * @fn const auto&& get(const tuple<Ts...>&& t)
	 * @brief Get i-th element
	 */
	template <size_t i, typename... Ts>
	const auto&& get(const tuple<Ts...>&& t) {
		return __Get<i>::get(t);
	}

	/**
	 * @struct tuple_size
	 * @brief Number of entries in tuple
	 */
	template<typename... Types>
	struct tuple_size<tuple<Types...>> : integral_constant<size_t, sizeof...(Types)> { };

	/**
	 * @struct tuple_element
	 * @brief Get type of i-th entry
	 */
	template<size_t I, typename T>
	struct tuple_element;
 
	/**
	 * @struct tuple_element
	 * @brief Get type of i-th entry
	 */
	template<size_t I, typename Head, typename... Tail>
	struct tuple_element<I, tuple<Head, Tail...>> : tuple_element<I-1, tuple<Tail...>> { };
 
	/**
	 * @struct tuple_element
	 * @brief Get type of i-th entry
	 */
	template<typename Head, typename... Tail >
	struct tuple_element<0, tuple<Head, Tail...>> {
	   using type = Head;
	};

} /* namespace lib */

namespace std {
	template<typename T>
	struct tuple_size;

	template<typename... Types>
	struct tuple_size<lib::tuple<Types...>> : lib::tuple_size<lib::tuple<Types...>> {};

	template<size_t I, typename T>
	struct tuple_element;
 
	template<size_t I, typename... Types>
	struct tuple_element<I, lib::tuple<Types...>> : lib::tuple_element<I, lib::tuple<Types...>> { };
 
} /* namespace std */

#endif /* ifndef _INC_TUPLE_H_ */
