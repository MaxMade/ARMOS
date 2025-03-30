#ifndef _INC_LIST_H_
#define _INC_LIST_H_

#include <new.h>
#include <cerrno.h>
#include <cstddef.h>
#include <utility.h>
#include <iterator.h>
#include <algorithm.h>

namespace lib {

	/**
	 * @class list
	 * @brief Double Linked List
	 */
	template<typename T>
	class list {
		private:
			/**
			 * @struct list_entry
			 * @brief Entry within list
			 */
			struct list_entry {
				/**
				 * @var next
				 * @brief Next Entry
				 */
				list_entry* next;

				/**
				 * @var prev
				 * @brief Previous entry
				 */
				list_entry* prev;

				/**
				 * @var val
				 * @brief Value
				 */
				T val;

				/**
				 * @fn list_entry(const T& val)
				 * @brief Construct entry from value
				 */
				list_entry(const T& val) : next(nullptr), prev(nullptr), val(val) { }

				/**
				 * @fn list_entry(T&& val)
				 * @brief Construct entry from value
				 */
				list_entry(T&& val) : next(nullptr), prev(nullptr), val(move(val)) { }
			};

			/**
			 * @var head
			 * @brief Pointer to list start
			 */
			list_entry* head;

			/**
			 * @var tail
			 * @brief Pouinter to list end
			 */
			list_entry* tail;

			/**
			 * @var len
			 * @brief Length of list
			 */
			size_t len;

		public:
			/**
			 * @class list_iterator
			 * @brief Bidirectional iterator
			 */
			template<typename U>
			class list_iterator {
				private:
					list_entry* head;
					list_entry* tail;
					list_entry* ptr;

					bool isValid() const {
						return ptr != nullptr;
					}

					list_entry* getEntry() const {
						return ptr;
					}

					friend class list;

				public:
					list_iterator(list_entry* head, list_entry* tail, list_entry* ptr) : head(head), tail(tail), ptr(ptr) { }

					list_iterator& operator++() {
						ptr = ptr->next;
						return *this;
					}

					list_iterator operator++(int) {
						list_iterator ret(*this);
						this->ptr = ptr->next;
						return ret;
					}

					list_iterator operator+(int n) {
						list_iterator tmp(*this);
						for (int i = 0; i < n; i++) {
							++tmp;
						}

						for (int i = 0; i > n; i--) {
							--tmp;
						}

						return tmp;
					}

					list_iterator& operator+=(int n) {
						for (int i = 0; i < n; i++) {
							++(*this);
						}

						for (int i = 0; i > n; i--) {
							--(*this);
						}

						return *this;
					}

					list_iterator& operator--() {
						if (ptr != nullptr) {
							ptr = ptr->prev;
						} else {
							ptr = tail;
						}

						return *this;
					}

					list_iterator operator--(int) {
						list_iterator ret(*this);
						if (ptr != nullptr) {
							this->ptr = ptr->prev;
						} else {
							this->ptr = tail;
						}
						return ret;
					}

					list_iterator operator-(int n) {
						list_iterator tmp(*this);
						for (int i = 0; i < n; i++) {
							--tmp;
						}

						for (int i = 0; i > n; i--) {
							++tmp;
						}

						return tmp;
					}

					list_iterator& operator-=(int n) {
						for (int i = 0; i < n; i++) {
							--(*this);
						}

						for (int i = 0; i > n; i--) {
							++(*this);
						}

						return *this;
					}

					U& operator*() {
						return ptr->val;
					}

					bool operator==(const list_iterator& other) const {
						return ptr == other.ptr;
					}

					bool operator!=(const list_iterator& other) const {
						return ptr != other.ptr;
					}
			};

			using value_type = T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
			using size_type = size_t;
			using iterator = list_iterator<T>;
			using const_iterator = list_iterator<const T>;
			using reverse_iterator = lib::reverse_iterator<iterator>;
			using const_reverse_iterator = lib::reverse_iterator<const_iterator>;

			/**
			 * @fn list()
			 * @brief Create emtpty list
			 */
			list() : head(nullptr), tail(nullptr), len(0) { }

			list(const list& other) = delete;

			/**
			 * @fn list(list&& other)
			 * @brief Move list
			 */
			list(list&& other) {
				this->head = other.head;
				this->tail = other.tail;
				this->len = other.len;

				other.head = nullptr;
				other.tail = nullptr;
				other.len = 0;
			}

			/**
			 * @fn ~list()
			 * @brief Destruct list
			 */
			~list() {
				clear();
			}

			/**
			 * @fn bool empty() const
			 * @brief Check if list is empty
			 */
			bool empty() const {
				return len == 0;
			}

			/**
			 * @fn size_type size()
			 * @brief Get size
			 */
			size_type size() const {
				return len;
			}

			reference front() {
				return *head;
			}

			const_reference front() const {
				return *head;
			}

			reference back() {
				return *tail;
			}

			const_reference back() const {
				return *tail;
			}

			iterator begin() {
				return iterator(head, tail, head);
			}

			iterator end() {
				return iterator(head, tail, nullptr);
			}

			const_iterator cbegin() const {
				return const_iterator(head, tail, head);
			}

			const_iterator cend() const {
				return const_iterator(head, tail, nullptr);
			}

			reverse_iterator rbegin() {
				return reverse_iterator(end());
			}

			reverse_iterator rend() {
				return reverse_iterator(begin());
			}

			const_reverse_iterator crbegin() const {
				const_reverse_iterator ret(const_iterator(head, tail, nullptr));
				return ret;
			}

			const_reverse_iterator crend() const {
				const_reverse_iterator ret(const_iterator(head, tail, head));
				return ret;
			}

			/**
			 * @fn void clear()
			 * @brief Clear list
			 */
			void clear() {
				auto drag = head;
				while (drag != nullptr) {
					auto dragNext = drag->next;
					delete drag;
					drag = dragNext;
				}

				head = nullptr;
				tail = nullptr;
				len = 0;
			}

			/**
			 * @fn int insert(const_iterator pos, const T& value)
			 * @brief Insert entry at given position
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int insert(const_iterator pos, const T& value) {
				/* Allocate new entry */
				list_entry* newEntry = new list_entry(value);
				if (!newEntry)
					return -ENOMEM;

				insert(pos, newEntry);

				return 0;
			}

			/**
			 * @fn int insert(const_iterator pos, T&& value)
			 * @brief Insert entry at given position
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int insert(const_iterator pos, T&& value) {
				list_entry* newEntry = new list_entry(move(value));
				if (!newEntry)
					return -ENOMEM;

				insert(pos, newEntry);

				return 0;
			}

			/**
			 * @fn bool emplace(const_iterator pos, Args&&... args)
			 * @brief Emplace entry at given position
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			template< class... Args >
			int emplace(const_iterator pos, Args&&... args) {
				list_entry* newEntry = new list_entry(lib::forward<Args>(args)...);
				if (!newEntry)
					return -ENOMEM;

				insert(pos, newEntry);

				return 0;
			}

			/**
			 * @fn bool erase(const_iterator pos)
			 * @brief Remove entry
			 * @return
			 *
			 *	- true  - Success
			 *	- false - Failure
			 */
			bool erase(const_iterator pos) {
				if (!pos.isValid())
					return false;

				list_entry* entry = pos.getEntry();
				list_entry* nextEntry = entry->next;
				list_entry* prevEntry = entry->prev;

				if (prevEntry) {
					prevEntry->next = nextEntry;
				} else {
					head = nextEntry;
				}

				if (nextEntry) {
					nextEntry->prev = prevEntry;
				} else {
					tail = prevEntry;
				}

				len--;

				delete entry;

				return true;
			}

			/**
			 * @fn int push_back(const T& value)
			 * @brief Push new entry at back
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			bool push_back(const T& value) {
				list_entry* newEntry = new list_entry(value);
				if (!newEntry)
					return false;

				insert(cend(), newEntry);

				return true;
			}

			/**
			 * @fn int push_back(T&& value)
			 * @brief Push new entry at back
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			int push_back(T&& value) {
				list_entry* newEntry = new list_entry(move(value));
				if (!newEntry)
					return -ENOMEM;

				insert(cend(), newEntry);

				return 0;
			}

			/**
			 * @fn int emplace_back(Args&&... args)
			 * @brief Emplace new entry at back
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			template<class... Args >
			int emplace_back(Args&&... args) {
				list_entry* newEntry = new list_entry(lib::forward<Args>(args)...);
				if (!newEntry)
					return -ENOMEM;

				insert(cend(), newEntry);

				return 0;
			}

			/**
			 * @fn bool pop_back()
			 * @brief Remove entry at end
			 * @return
			 *
			 *	- true  - Success
			 *	- false - Failure
			 */
			bool pop_back() {
				return erase(prev(cend()));
			}

			/**
			 * @fn int push_front(const T& value)
			 * @brief Push new entry at front
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			int push_front(const T& value) {
				list_entry* newEntry = new list_entry(value);
				if (!newEntry)
					return -ENOMEM;

				insert(cbegin(), newEntry);

				return 0;
			}

			/**
			 * @fn int push_front(T&& value)
			 * @brief Push new entry at front
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			int push_front(T&& value) {
				list_entry* newEntry = new list_entry(move(value));
				if (!newEntry)
					return -ENOMEM;

				insert(cbegin(), newEntry);

				return 0;
			}

			/**
			 * @fn int emplace_front(Args&&... args)
			 * @brief Emplace new entry at front
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			template<class... Args >
			int emplace_front(Args&&... args) {
				list_entry* newEntry = new list_entry(lib::forward<Args>(args)...);
				if (!newEntry)
					return -ENOMEM;

				insert(cbegin(), newEntry);
				return 0;
			}

			bool pop_front() {
				return erase(cbegin());
			}

			/**
			 * @fn void remove(const T& value)
			 * @brief Remvoe matching entries
			 */
			void remove(const T& value) {
				auto it = cbegin();
				auto itEnd = cend();

				while (it != itEnd) {
					if (*it == value) {
						auto itErase = it++;
						erase(itErase);
						continue;
					}

					++it;
				}
			}

			/**
			 * @fn void remove_if(UnaryPredicate p)
			 * @brief Remove any entry matching predicate p
			 */
			template<typename UnaryPredicate>
			void remove_if(UnaryPredicate p) {
				auto it = cbegin();
				auto itEnd = cend();

				while (it != itEnd) {
					if (p(*it)) {
						auto itErase = it++;
						erase(itErase);
						continue;
					}

					++it;
				}
			}

			/**
			 * @fn void swap(list& other)
			 * @brief Swap two lists in O(1)
			 */
			void swap(list& other) {
				lib::swap(this->head, other.head);
				lib::swap(this->tail, other.tail);
				lib::swap(this->len, other.len);
			}

			/**
			 * @fn void reverse()
			 * @brief Reverse list in O(n)
			 */
			void reverse() {
				auto drag = head;
				while (drag != nullptr) {
					auto dragNext = drag->next;
					lib::swap(drag->next, drag->prev);
					drag = dragNext;
				}

				lib::swap(head, tail);
			}

			/**
			 * @fn void sort()
			 * @brief Sort list in O(n^2)
			 */
			void sort() {
				lib::sort(begin(), end());
			}

		private:
			/**
			 * @fn void insert(const_iterator pos, list_entry* entry)
			 * @brief Insert entry into list at position pos
			 */
			void insert(const_iterator pos, list_entry* entry) {
				len++;

				list_entry* nextEntry = nullptr;
				list_entry* prevEntry = nullptr;

				if (pos.isValid()) {
					nextEntry = pos.getEntry();
					prevEntry = nextEntry->prev;
				} else {
					prevEntry = tail;
				}

				entry->next = nextEntry;
				entry->prev = prevEntry;

				if (prevEntry) {
					prevEntry->next = entry;
				} else {
					head = entry;
				}

				if (nextEntry) {
					nextEntry->prev = entry;
				} else {
					tail = entry;
				}
			}
	};

} /* namespace lib */

#endif /* ifndef _INC_LIST_H_ */
