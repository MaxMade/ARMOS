#ifndef _INC_MEMORY_H_
#define _INC_MEMORY_H_

#include <new.h>
#include <utility.h>
#include <type_traits.h>

/**
 * @file memory.h
 * @brief Smart Pointers
 */

namespace lib {

	/**
	 * @class unique_ptr
	 * @brief Unique Smart Pointer
	 */
	template<typename T>
	class unique_ptr {
		private:
			/**
			 * @var ptr
			 * @brief Underlying pointer
			 */
			T* ptr;

		public:
			/**
			 * @typedef pointer
			 * @brief Pointer to T
			 */
			using pointer = T*;

			/**
			 * @typedef element_type
			 * @brief Element itself
			 */
			using element_type = T;

			/**
			 * @fn unique_ptr()
			 * @brief Create unique_ptr and initialize with nullptr
			 */
			unique_ptr() : ptr(nullptr) {}

			/**
			 * @fn explicit unique_ptr(T* ptr)
			 * @brief Create unique_ptr from pointer
			 */
			explicit unique_ptr(T* ptr) : ptr(ptr) { }

			unique_ptr(const unique_ptr&) = delete;

			/**
			 * @fn unique_ptr(unique_ptr&& other)
			 * @brief Move construct unique_ptr
			 */
			unique_ptr(unique_ptr&& other) {
				ptr = other.ptr;
				other.ptr = nullptr;
			}

			/**
			 * @fn ~unique_ptr()
			 * @brief Destruct and free underlying memory
			 */
			~unique_ptr() {
				if (ptr != nullptr)
					delete ptr;
			}

			unique_ptr& operator=(const unique_ptr&) = delete;

			/**
			 * @fn unique_ptr& operator=(unique_ptr&& other)
			 * @brief Move Assignment
			 */
			unique_ptr& operator=(unique_ptr&& other) {
				if (ptr != nullptr)
					delete ptr;

				ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}

			/**
			 * @fn void reset(pointer p = pointer())
			 * @brief Reassign new pointer and free old one
			 */
			void reset(pointer p = pointer()) {
				T* tmp = ptr;
				ptr = p;
				if (tmp != nullptr)
					delete tmp;
			}

			/**
			 * @fn void swap(unique_ptr& other)
			 * @brief Swap content with other unique_ptr
			 */
			void swap(unique_ptr& other) {
				lib::swap(this->ptr, other.ptr);
			}

			/**
			 * @fn pointer get() const
			 * @brief Get underlying pointer
			 */
			pointer get() const {
				return ptr;
			}

			/**
			 * @fn pointer operator->() const
			 * @brief Get underlying pointer
			 */
			pointer operator->() const {
				return ptr;
			}

			/**
			 * @fn explicit operator bool() const
			 * @brief Check if underlying pointer is not null
			 */
			explicit operator bool() const {
				return ptr != nullptr;
			}

			/**
			 * @fn typename lib::add_lvalue_reference<T>::type operator*() const
			 * @brief Get reference to underlying object
			 * @warning Unspecified behaviour in case of an underlying nullptr
			 */
			typename lib::add_lvalue_reference<T>::type operator*() const {
				return *ptr;
			}
	};

	template<typename T, typename... Args>
	unique_ptr<T> make_unique( Args&&... args ) {
		return unique_ptr(new T(forward<Args>(args)...));
	}

	template<typename T >
	void swap(unique_ptr<T>& lhs, unique_ptr<T>& rhs) {
		lhs.swap(rhs);
	}

} /* namespace lib */

#endif /* ifndef _INC_MEMORY_H_ */
