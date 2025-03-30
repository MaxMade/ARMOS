#ifndef _INC_ATOMIC_H_
#define _INC_ATOMIC_H_

#include <type_traits.h>

/**
 * @file atomic.h
 * @brief Atomic operations
 */

namespace lib {

	/**
	 * @typedef memory_order
	 * @brief Possible memory orders
	 */
	typedef enum memory_order {
		memory_order_relaxed = __ATOMIC_RELAXED, /**< No synchronization or ordering constraints imposed on other reads/writes, but atomicity is guaranteed */
		memory_order_consume = __ATOMIC_CONSUME, /**< No read/writes can be reordered before this load in current thread and all atomic writes to data-dependent variables in other threads are visible */
		memory_order_acquire = __ATOMIC_ACQUIRE, /**< No read/writes can be reordered before this load in current thread and all writes to data-dependent variables in other threads are visible */
		memory_order_release = __ATOMIC_RELEASE, /**< No read/writes can be reordered after this load in current thread and all writes to data-dependent variables in other threads are visible */
		memory_order_acq_rel = __ATOMIC_ACQ_REL, /**< Combination of acquire/release semantic */
		memory_order_seq_cst = __ATOMIC_SEQ_CST  /**< Combination of acquire/release semantic with total order */
	} memory_order;

	/**
	 * @struct atomic_flag
	 * @brief Atomic boolean type
	 */
	struct atomic_flag {
		/**
		 * @var flag
		 * @brief Underlying boolean value
		 */
		bool flag;

		/**
		 * @fn atomic_flag()
		 * @brief Trivial default constructor
		 */
		atomic_flag() = default;

		atomic_flag(const atomic_flag&) = delete;

		atomic_flag(atomic_flag&&) = delete;

		atomic_flag& operator=(const atomic_flag&) = delete;

		atomic_flag& operator=(atomic_flag&&) = delete;

		/**
		 * @fn void clear(memory_order order = memory_order_seq_cst)
		 * @brief Set to false atomically
		 */
		void clear(memory_order order = memory_order_seq_cst);

		/**
		 * @fn bool test_and_set(memory_order order = memory_order_seq_cst)
		 * @brief Set to true and return previous value atomically
		 */
		bool test_and_set(memory_order order = memory_order_seq_cst);
	};

	/**
	 * @fn bool atomic_flag_test_and_set(atomic_flag* p)
	 * @brief Set to true and return previous value atomically
	 */
	bool atomic_flag_test_and_set(atomic_flag* p);

	/**
	 * @fn bool atomic_flag_test_and_set_explicit(atomic_flag* p, memory_order order)
	 * @brief Set to true and return previous value atomically
	 */
	bool atomic_flag_test_and_set_explicit(atomic_flag* p, memory_order order);

	/**
	 * @fn void atomic_flag_clear(atomic_flag* p)
	 * @brief Set to false atomically
	 */
	void atomic_flag_clear(atomic_flag* p);

	/**
	 * @fn void atomic_flag_clear_explicit(atomic_flag* p, memory_order order)
	 * @brief Set to false atomically
	 */
	void atomic_flag_clear_explicit(atomic_flag* p, memory_order order);


	template<typename T>
	struct atomicHelper {

		using value_type = T;

		using difference_type = value_type;

		T t;

		atomicHelper() {}

		atomicHelper(const atomicHelper&) = delete;

		atomicHelper(atomicHelper&&) = delete;

		explicit atomicHelper(T desired) : t(desired) { }

		T operator=(const atomicHelper&) = delete;

		T operator=(atomicHelper&&) = delete;

		void store(T desired, memory_order order = memory_order_seq_cst) {
			__atomic_store_n(&t, desired, order);
		}

		T load(memory_order order = memory_order_seq_cst) const {
			return __atomic_load_n(&t, order);
		}

		T exchange(T desired, memory_order order = memory_order_seq_cst) {
			return __atomic_exchange_n(&t, desired, order);
		}

		bool compare_exchange_weak(T& expected, T desired, memory_order success, memory_order failure) {
			return __atomic_compare_exchange_n(&t, &expected, desired, true, success, failure);
		}

		bool compare_exchange_weak(T& expected, T desired, memory_order order = memory_order_seq_cst) {
			return __atomic_compare_exchange_n(&t, &expected, desired, true, order, order);
		}

		bool compare_exchange_strong(T& expected, T desired, memory_order success, memory_order failure) {
			return __atomic_compare_exchange_n(&t, &expected, desired, false, success, failure);
		}

		bool compare_exchange_strong(T& expected, T desired, memory_order order = memory_order_seq_cst) {
			return __atomic_compare_exchange_n(&t, &expected, desired, false, order, order);
		}
	};

	/**
	 * @struct atomic
	 * @brief Atomic class template
	 * @warning T must be an integral value
	 */
	template<typename T>
	struct atomic : public atomicHelper<T> {

		static_assert(is_integral<T>(), "T must be integral value");

		/**
		 * @fn atomic
		 * @brief Default constructor
		 */
		atomic() : atomicHelper<T>() { }

		/**
		 * @fn atomic(T desired)
		 * @brief Constructor and initialize with given value
		 */
		explicit atomic(T desired) : atomicHelper<T>(desired) { }

		/**
		 * @fn T fetch_add(T arg, memory_order order = memory_order_seq_cst)
		 * @brief Add arg and return previous value atomically
		 */
		T fetch_add(T arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_add(&this->t, arg, order);
		}

		/**
		 * @fn T fetch_sub(T arg, memory_order order = memory_order_seq_cst)
		 * @brief Subtract arg and return previous value atomically
		 */
		T fetch_sub(T arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_sub(&this->t, arg, order);
		}

		/**
		 * @fn T fetch_and(T arg, memory_order order = memory_order_seq_cst)
		 * @brief And arg and return previous value atomically
		 */
		T fetch_and(T arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_and(&this->t, arg, order);
		}

		/**
		 * @fn T fetch_or(T arg, memory_order order = memory_order_seq_cst)
		 * @brief Or arg and return previous value atomically
		 */
		T fetch_or(T arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_or(&this->t, arg, order);
		}

		/**
		 * @fn T fetch_xor(T arg, memory_order order = memory_order_seq_cst)
		 * @brief Xor arg and return previous value atomically
		 */
		T fetch_xor(T arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_xor(&this->t, arg, order);
		}
	};

	/**
	 * @struct atomic
	 * @brief Atomic wrapper for pointers
	 */
	template<typename T>
	struct atomic<T*> : public atomicHelper<T*> {
		using difference_type = ptrdiff_t;

		/**
		 * @fn atomic
		 * @brief Default constructor
		 */
		atomic() : atomicHelper<T*>() { }

		/**
		 * @fn atomic(T* desired)
		 * @brief Construct and initialize with given value
		 */
		explicit atomic(T* desired) : atomicHelper<T*>(desired) { }

		/**
		 * @fn T* fetch_add(ptrdiff_t arg, memory_order order = memory_order_seq_cst)
		 * @brief Add arg and return previous value atomically
		 */
		T* fetch_add(ptrdiff_t arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_add(&this->t, arg * sizeof(T), order);
		}


		/**
		 * @fn T* fetch_sub(ptrdiff_t arg, memory_order order = memory_order_seq_cst)
		 * @brief Subtract arg and return previous value atomically
		 */
		T* fetch_sub(ptrdiff_t arg, memory_order order = memory_order_seq_cst) {
			return __atomic_fetch_sub(&this->t, arg * sizeof(T), order);
		}
	};

	/**
	 * @fn void atomic_store(atomic<T>* obj, typename atomic<T>::value_type desr)
	 * @brief Atomic store
	 */
	template<typename T>
	void atomic_store(atomic<T>* obj, typename atomic<T>::value_type desr) {
		obj->store(desr);
	}

	/**
	 * @fn void atomic_store_explicit(atomic<T>* obj, typename atomic<T>::value_type desr, memory_order order)
	 * @brief Atomic store with given memory order
	 */
	template<typename T>
	void atomic_store_explicit(atomic<T>* obj, typename atomic<T>::value_type desr, memory_order order) {
		obj->store(desr, order);
	}

	/**
	 * @fn T atomic_load(const atomic<T>* obj)
	 * @brief Atomic load
	 */
	template<typename T>
	T atomic_load(const atomic<T>* obj) {
		return obj->load();
	}

	/**
	 * @fn T atomic_load(const atomic<T>* obj, memory_order order)
	 * @brief Atomic load with given memory order
	 */
	template<typename T>
	T atomic_load(const atomic<T>* obj, memory_order order) {
		return obj->load(order);
	}

	/**
	 * @fn T atomic_exchange(atomic<T>* obj, typename atomic<T>::value_type desr)
	 * @brief Atomic exchange
	 */
	template<typename T>
	T atomic_exchange(atomic<T>* obj, typename atomic<T>::value_type desr) {
		return obj->exchange(desr);
	}

	/**
	 * @fn T atomic_exchange_explicit(atomic<T>* obj, typename atomic<T>::value_type desr, memory_order order)
	 * @brief Atomic exchange with given memory order
	 */
	template<typename T>
	T atomic_exchange_explicit(atomic<T>* obj, typename atomic<T>::value_type desr, memory_order order) {
		return obj->exchange(desr, order);
	}

	/**
	 * @fn bool atomic_compare_exchange_weak(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired)
	 * @brief Atomic compare and exchange (weak)
	 */
	template<typename T>
	bool atomic_compare_exchange_weak(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired) {
		return obj->compare_exchange_weak(*expected, desired);
	}

	/**
	 * @fn bool atomic_compare_exchange_strong(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired)
	 * @brief Atomic compare and exchange (strong)
	 */
	template<typename T>
	bool atomic_compare_exchange_strong(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired) {
		return obj->compare_exchange_strong(*expected, desired);
	}

	/**
	 * @fn bool atomic_compare_exchange_weak_explicit(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired, memory_order succ, memory_order fail)
	 * @brief Atomic compare and exchange with given memory order
	 */
	template<typename T>
	bool atomic_compare_exchange_weak_explicit(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired, memory_order succ, memory_order fail) {
		return obj->compare_exchange_weak_explicit(*expected, desired, succ, fail);
	}

	/**
	 * @fn bool atomic_compare_exchange_strong_explicit(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired, memory_order succ, memory_order fail)
	 * @brief Atomic compare and exchange with given memory order
	 */
	template<typename T>
	bool atomic_compare_exchange_strong_explicit(atomic<T>* obj, typename atomic<T>::value_type* expected, typename atomic<T>::value_type desired, memory_order succ, memory_order fail) {
		return obj->compare_exchange_strong_explicit(*expected, desired, succ, fail);
	}

	/**
	 * @fn T atomic_fetch_add(atomic<T>* obj, typename atomic<T>::difference_type arg)
	 * @brief Add value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_add(atomic<T>* obj, typename atomic<T>::difference_type arg) {
		return obj->fetch_add(arg);
	}

	/**
	 * @fn T atomic_fetch_add_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order)
	 * @brief Add value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_add_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order) {
		return obj->fetch_add_explicit(arg, order);
	}

	/**
	 * @fn T atomic_fetch_sub(atomic<T>* obj, typename atomic<T>::difference_type arg)
	 * @brief Subtract value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_sub(atomic<T>* obj, typename atomic<T>::difference_type arg) {
		return obj->fetch_sub(arg);
	}

	/**
	 * @fn T atomic_fetch_sub_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order)
	 * @brief Subtract value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_sub_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order) {
		return obj->fetch_sub_explicit(arg, order);
	}

	/**
	 * @fn T atomic_fetch_and(atomic<T>* obj, typename atomic<T>::difference_type arg)
	 * @brief And value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_and(atomic<T>* obj, typename atomic<T>::value_type arg) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_and(arg);
	}

	/**
	 * @fn T atomic_fetch_and_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order)
	 * @brief And value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_and_explicit(atomic<T>* obj, typename atomic<T>::value_type arg, memory_order order) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_and_explicit(arg, order);
	}

	/**
	 * @fn T atomic_fetch_or(atomic<T>* obj, typename atomic<T>::difference_type arg)
	 * @brief Or value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_or(atomic<T>* obj, typename atomic<T>::value_type arg) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_or(arg);
	}

	/**
	 * @fn T atomic_fetch_or_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order)
	 * @brief Or value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_or_explicit(atomic<T>* obj, typename atomic<T>::value_type arg, memory_order order) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_or_explicit(arg, order);
	}

	/**
	 * @fn T atomic_fetch_xor(atomic<T>* obj, typename atomic<T>::difference_type arg)
	 * @brief Xor value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_xor(atomic<T>* obj, typename atomic<T>::value_type arg) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_xor(arg);
	}

	/**
	 * @fn T atomic_fetch_xor_explicit(atomic<T>* obj, typename atomic<T>::difference_type arg, memory_order order)
	 * @brief Xor value and return previous value atomically
	 */
	template<typename T>
	T atomic_fetch_xor_explicit(atomic<T>* obj, typename atomic<T>::value_type arg, memory_order order) {
		static_assert(is_integral<T>(), "T must be integral value");
		return obj->fetch_xor_explicit(arg, order);
	}

} /* namespace lib */

#endif /* ifndef _INC_ATOMIC_H_ */
