#ifndef _INC_MEMORY_H_
#define _INC_MEMORY_H_

#include <new.h>
#include <atomic.h>
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


	/**
	 * @class shared_ptr
	 * @brief Shared Pointer
	 */
	template<typename T>
	class shared_ptr {
		private:
			/**
			 * @var val
			 * @brief Pointer value
			 */
			T* val;

			/**
			 * @var ref
			 * @brief Reference count
			 */
			atomic<size_t>* ref;

		public:
			/**
			 * @fn shared_ptr()
			 * @brief Create invalid shared pointer
			 */
			shared_ptr() : val(nullptr), ref(nullptr) {}

			/**
			 * @fn explicit shared_ptr(U* ptr)
			 * @brief Create shared pointer from normal pointer
			 */
			template<typename U>
			explicit shared_ptr(U* ptr) {
				val = ptr;
				ref = new atomic<size_t>(1);

				if (!val || !ref) {
					delete val;
					delete ref;
				}
			}

			/**
			 * @fn shared_ptr(const shared_ptr<U>& r)
			 * @brief Copy constructor
			 */
			template<typename U>
			shared_ptr(const shared_ptr<U>& r) {
				val = r.val;
				ref = r.ref;
				ref->fetch_add(1);
			}

			/**
			 * @fn shared_ptr(shared_ptr<U>&& r)
			 * @brief Move constructor
			 */
			template<typename U>
			shared_ptr(shared_ptr<U>&& r) {
				val = r.val;
				ref = r.ref;

				r.val = nullptr;
				r.ref = 0;
			}

			/**
			 * @fn ~shared_ptr()
			 * @brief Destructor
			 */
			~shared_ptr() {
				if (ref->fetch_sub(1) == 1) {
					delete val;
					delete ref;
				}
			}

			/**
			 * @fn shared_ptr& operator=(const shared_ptr<U>& r)
			 * @brief Copy assignment
			 */
			template<class U>
			shared_ptr& operator=(const shared_ptr<U>& r) {
				shared_ptr<T>(r).swap(*this);
				return *this;
			}

			/**
			 * @fn shared_ptr& operator=(shared_ptr<U>&& r)
			 * @brief Move assignment
			 */
			template<class U>
			shared_ptr& operator=(shared_ptr<U>&& r) {
				shared_ptr<T>(move(r)).swap(*this);
				return *this;
			}

			/**
			 * @fn operator bool() const
			 * @brief Check if shared_pointer contains valid pointer
			 */
			operator bool() const {
				return val != nullptr;
			}

			/**
			 * @fn void swap(shared_ptr& r)
			 * @brief Swap two shared_pointers
			 */
			void swap(shared_ptr& r) {
				swap(this->val, r.val);
				swap(this->ref, r.ref);
			}

			/**
			 * @fn T* get() const
			 * @brief Get underlying pointer
			 */
			T* get() const {
				return val;
			}

			/**
			 * @fn T& operator*() const
			 * @brief Get underlying pointer
			 */
			T& operator*() const {
				return *val;
			}

			/**
			 * @fn T* operator->() const
			 * @brief Get underlying pointer
			 */
			T* operator->() const {
				return val;
			}

			/**
			 * @fn size_t use_count() const
			 * @brief Get underlying pointer
			 */
			size_t use_count() const {
				auto ret = ref->load();
				return ret;
			}
	};

} /* namespace lib */

#endif /* ifndef _INC_MEMORY_H_ */
