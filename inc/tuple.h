#ifndef _INC_TUPLE_H_
#define _INC_TUPLE_H_

#include <cstddef.h>
#include <utility.h>

/**
 * @file tuple.h
 * @brief Tuple class and helpers
 */

namespace lib {
	/* forward declartion */

	template<typename First, typename... Rest>
	struct tuple;

	template<typename First, typename... Rest>
	void swap(tuple<First, Rest...>& lhs, tuple<First, Rest...>& rhs);

	template<typename First, typename... Rest>
	void assign(tuple<First, Rest...>& lhs, const tuple<First, Rest...>& rhs);

	template<typename First, typename... Rest>
	void assign(tuple<First, Rest...>& lhs, tuple<First, Rest...>&& rhs);

	/* tuple defintion */

	//! @cond Doxygen_Suppress
	/**
	 * @struct tuple
	 * @brief Tuple
	 */
	template<typename First, typename... Rest>
	struct tuple : public tuple<Rest...> {
		tuple(First first, Rest... rest): tuple<Rest...>(rest...), first(first) {}

		/**
		 * @var first
		 * @brief Entry for current index
		 */
		First first;

		/**
		 * @fn void swap(tuple<First, Rest...>& other)
		 * @brief Swap contents
		 */
		void swap(tuple<First, Rest...>& other) {
			lib::swap(*this, other);
		}

		/**
		 * @fn tuple& operator=(const tuple& other)
		 * @brief Copy Assignment
		 */
		tuple& operator=(const tuple& other) {
			assign(*this, other);
			return *this;
		}

		/**
		 * @fn tuple& operator=(tuple&& other)
		 * @brief Move Assignment
		 */
		tuple& operator=(tuple&& other) {
			assign(*this, move(other));
			return *this;
		}
	};
	//! @endcond

	template<typename First, typename... Rest>
	tuple(First, Rest...) -> tuple<First, Rest...>;

	template<typename First>
	struct tuple<First> {
		tuple(First first): first(first) {}
		First first;

		void swap(tuple<First>& other) {
			swap(this->first, other.first);
		}

		tuple& operator=(const tuple& other) {
			assign(*this, other);
			return *this;
		}

		tuple& operator=(tuple&& other) {
			assign(*this, move(other));
			return *this;
		}
	};

	template<typename First>
	tuple(First) -> tuple<First>;

	/* get implementation */

	template<size_t index, typename First, typename... Rest>
	struct _GetImpl {
		static auto& value(tuple<First, Rest...>& t) {
			return _GetImpl<index - 1, Rest...>::value(t);
		}

		static const auto& value(const tuple<First, Rest...>& t) {
			return _GetImpl<index - 1, Rest...>::value(t);
		}
	};

	template<typename First, typename... Rest>
	struct _GetImpl<0, First, Rest...> {
		static First& value(tuple<First, Rest...>& t) {
			return t.first;
		}

		static const First& value(const tuple<First, Rest...>& t) {
			return t.first;
		}
	};

	/**
	 * @fn auto& get(tuple<First, Rest...>& t)
	 * @brief Get value at given index
	 */
	template<size_t index, typename First, typename... Rest>
	auto& get(tuple<First, Rest...>& t) {
		return _GetImpl<index, First, Rest...>::value(t);
	}

	/**
	 * @fn const auto& get(const tuple<First, Rest...>& t)
	 * @brief Get value at given index
	 */
	template<size_t index, typename First, typename... Rest>
	const auto& get(const tuple<First, Rest...>& t) {
		return _GetImpl<index, First, Rest...>::value(t);
	}

	/* assign implementation */

	template<size_t index, typename First, typename... Rest>
	struct _assignImpl {
		static void assign(tuple<First, Rest...>& lhs, const tuple<First, Rest...>& rhs) {
			auto& _lhs = get<index>(lhs);
			auto& _rhs = get<index>(rhs);

			_lhs = _rhs;

			_assignImpl<index - 1, First, Rest...>::assign(lhs, rhs);
		}

		static void assign(tuple<First, Rest...>& lhs, tuple<First, Rest...>&& rhs) {
			auto& _lhs = get<index>(lhs);
			auto& _rhs = get<index>(rhs);

			_lhs = move(_rhs);

			_assignImpl<index - 1, First, Rest...>::assign(lhs, move(rhs));
		}
	};

	template<typename First, typename... Rest>
	struct _assignImpl<0, First, Rest...> {
		static void assign(tuple<First, Rest...>& lhs, const tuple<First, Rest...>& rhs) {
			auto& _lhs = get<0>(lhs);
			auto& _rhs = get<0>(rhs);

			_lhs = _rhs;
		}

		static void assign(tuple<First, Rest...>& lhs, tuple<First, Rest...>&& rhs) {
			auto& _lhs = get<0>(lhs);
			auto& _rhs = get<0>(rhs);

			_lhs = move(_rhs);
		}
	};


	template<typename First, typename... Rest>
	void assign(tuple<First, Rest...>& lhs, const tuple<First, Rest...>& rhs) {
		_assignImpl<sizeof...(Rest), First, Rest...>::assign(lhs, rhs);
	}

	template<typename First, typename... Rest>
	void assign(tuple<First, Rest...>& lhs, tuple<First, Rest...>&& rhs) {
		_assignImpl<sizeof...(Rest), First, Rest...>::assign(lhs, move(rhs));
	}


	/* swap implementation */

	template<size_t index, typename First, typename... Rest>
	struct _swapImpl {
		static void swap(tuple<First, Rest...>& lhs, tuple<First, Rest...>& rhs) {
			auto& _lhs = get<index>(lhs);
			auto& _rhs = get<index>(rhs);

			swap(_lhs, _rhs);

			_swapImpl<index - 1, First, Rest...>::swap(lhs, rhs);
		}
	};

	template<typename First, typename... Rest>
	struct _swapImpl<0, First, Rest...> {
		static void swap(tuple<First, Rest...>& lhs, tuple<First, Rest...>& rhs) {
			auto& _lhs = get<0>(lhs);
			auto& _rhs = get<0>(rhs);

			swap(_lhs, _rhs);
		}
	};


	/**
	 * @fn void swap(tuple<First, Rest...>& lhs, tuple<First, Rest...>& rhs)
	 * @brief Swap contents of two tuples
	 */
	template<typename First, typename... Rest>
	void swap(tuple<First, Rest...>& lhs, tuple<First, Rest...>& rhs) {
		_swapImpl<sizeof...(Rest), First, Rest...>::swap(lhs, rhs);
	}

	/* tuple_size implementation */

	template<typename T>
	class tuple_size {
	};

	/**
	 * @struct tuple_size
	 * @brief Get tuple size
	 */
	template<typename...T>
	struct tuple_size<tuple<T...>> {
	   public:
		   /**
			* @typedef value_type
			* @brief Type of value member
			*/
		   using value_type = size_t;

		   /**
			* @var value
			* @brief Size of tuple
			*/
		   constexpr static size_t value = sizeof...(T);

		   /**
			* @fn operator size_t() const
			* @brief Overloaded size cast
			*/
		   operator size_t() const {
			   return value;
		   }

		   /**
			* @fn value_type operator() ()
			* @brief Get size
			*/
		   value_type operator() () {
				return value;
		   }
	};

} /* namespace lib */

#endif /* ifndef _INC_TUPLE_H_ */
