#ifndef _INC_FUNCTIONAL_H_
#define _INC_FUNCTIONAL_H_

#include <memory.h>
#include <utility.h>

/**
 * @file functional.h
 * @brief Function objects
 */

namespace lib {

	/**
	 * @class function
	 * @brief Unspecified function
	 */
	template <typename T>
	class function;

	/**
	 * @class function
	 * @brief Generic function object
	 */
	template <typename ReturnValue, typename... Args>
	class function<ReturnValue(Args...)> {
		private:
			class ICallable {
				public:
					virtual ~ICallable() = default;

					virtual unique_ptr<ICallable> clone() = 0;

					virtual ReturnValue invoke(Args...) = 0;
			};

			template <typename T>
			class Callable : public ICallable {
				private:
					T t;

				public:
					Callable(const T& t) : t(t) {}

					Callable(const Callable& other) = default;

					Callable(Callable&& other) = default;

					~Callable() override = default;

					ReturnValue invoke(Args... args) override {
						return t(forward<Args>(args)...);
					}

					unique_ptr<ICallable> clone() override {
						return unique_ptr<ICallable>(new Callable(*this));
					}
			};

			/**
			 * @var fn
			 * @brief Underlying polymorphic function
			 */
			unique_ptr<ICallable> fn;

		public:
			/**
			 * @fn function()
			 * @brief Create invalid function object
			 */
			function() = default;

			/**
			 * @fn explicit function(T t)
			 * @brief Create from function object (might be lambda)
			 * @warning Creation must be followd by isValid() call
			 */
			template<typename T>
			explicit function(T t) {
				ICallable* f = new Callable<T>(t);
				fn.reset(f);
			}

			/**
			 * @fn function(const function& other)
			 * @brief Copy construction
			 * @warning Creation must be followd by isValid() call
			 */
			function(const function& other) {
				fn = move(other.fn->clone());
			}

			/**
			 * @fn function(function&& other)
			 * @brief Move construction
			 */
			function(function&& other) {
				fn = move(other.fn);
			}

			/**
			 * @fn operator bool() const
			 * @brief Check if function object contains valid callable object
			 */
			operator bool() const {
				return static_cast<bool>(fn);
			}

			/**
			 * @fn bool isValid() const
			 * @brief Check if function object contains valid callable object
			 */
			bool isValid() const {
				return static_cast<bool>(fn);
			}

			/**
			 * @fn function& operator=(T t)
			 * @brief Assign new function object
			 */
			template <typename T>
			function& operator=(T t) {
				ICallable* tmp = new Callable<T>(t);
				fn.reset(tmp);
				return *this;
			}

			/**
			 * @fn function& operator=(const function& other)
			 * @brief Copy Assignment
			 */
			function& operator=(const function& other) {
				fn = move(other.fn->clone());
				return *this;
			}

			/**
			 * @fn function& operator=(function&& other)
			 * @brief Move Assignment
			 */
			function& operator=(function&& other) {
				fn = move(other.fn);
				return *this;
			}

			/**
			 * @fn ReturnValue operator()(Args... args) const
			 * @brief Invoke underlying function object
			 * @warning Function object must be valid
			 */
			ReturnValue operator()(Args... args) const {
				return fn->invoke(forward<Args>(args)...);
			}
	};

	/**
	 * @class plus
	 * @brief Addition operation
	 */
	template<typename T>
	class plus {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a + b;
			}
	};

	/**
	 * @class minus
	 * @brief Subtraction operation
	 */
	template<typename T>
	class minus {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a - b;
			}
	};

	/**
	 * @class multiples
	 * @brief Multiplication operation
	 */
	template<typename T>
	class multiples {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a * b;
			}
	};

	/**
	 * @class divides
	 * @brief Division operation
	 */
	template<typename T>
	class divides {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a / b;
			}
	};

	/**
	 * @class modulus
	 * @brief Modulo operation
	 */
	template<typename T>
	class modulus {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a % b;
			}
	};

	/**
	 * @class negate
	 * @brief Negation operation
	 */
	template<typename T>
	class negate {
		public:
			using result_type = T;

			T operator()(const T& a) {
				return -a;
			}
	};

	/**
	 * @class equal_to
	 * @brief Equality comparision
	 */
	template<typename T>
	class equal_to {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a == b;
			}
	};

	/**
	 * @class not_equal_to
	 * @brief Non-Equality comparision
	 */
	template<typename T>
	class not_equal_to {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a != b;
			}
	};

	/**
	 * @class greater
	 * @brief Greater comparision
	 */
	template<typename T>
	class greater {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a > b;
			}
	};

	/**
	 * @class less
	 * @brief Less comparision
	 */
	template<typename T>
	class less {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a < b;
			}
	};

	/**
	 * @class greater_equal
	 * @brief Greater-Equal comparision
	 */
	template<typename T>
	class greater_equal {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a >= b;
			}
	};

	/**
	 * @class less_equal
	 * @brief Less-Equal comparision
	 */
	template<typename T>
	class less_equal {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a <= b;
			}
	};

	/**
	 * @class logical_and
	 * @brief And Operation
	 */
	template<typename T>
	class logical_and {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a && b;
			}
	};

	/**
	 * @class logical_or
	 * @brief Or Operation
	 */
	template<typename T>
	class logical_or {
		public:
			using result_type = T;

			bool operator()(const T& a, const T& b) {
				return a || b;
			}
	};

	/**
	 * @class logical_not
	 * @brief Not Operation
	 */
	template<typename T>
	class logical_not {
		public:
			using result_type = T;

			bool operator()(const T& a) {
				return !a;
			}
	};

	/**
	 * @class bit_and
	 * @brief Bitwise And Operation
	 */
	template<typename T>
	class bit_and {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a & b;
			}
	};

	/**
	 * @class bit_or
	 * @brief Bitwise Or Operation
	 */
	template<typename T>
	class bit_or {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a | b;
			}
	};

	/**
	 * @class bit_xor
	 * @brief Bitwise Xor Operation
	 */
	template<typename T>
	class bit_xor {
		public:
			using result_type = T;

			T operator()(const T& a, const T& b) {
				return a ^ b;
			}
	};

	/**
	 * @class bit_not
	 * @brief Bitwise Not Operation
	 */
	template<typename T>
	class bit_not {
		public:
			using result_type = T;

			T operator()(const T& a) {
				return ~a;
			}
	};

} /* namespace lib */

#endif /* ifndef _INC_FUNCTIONAL_H_ */
