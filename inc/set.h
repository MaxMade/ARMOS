#ifndef _INC_SET_H_
#define _INC_SET_H_

#include <cerrno.h>
#include <functional.h>
#include <kernel/adt/rbtree.h>

namespace lib {

	/**
	 * @class set
	 * @brief Associative Container
	 * @tparam Key Entry
	 * @tparam Compare Compare function
	 */
	template <typename Key, typename Compare = lib::less<Key>>
	class set {
		private:
			/**
			 * @typedef Node
			 * @brief Alias for underlying RBNode
			 */
			using Node = typename adt::RBTree<Key, Compare>::RBNode;

			/**
			 * @var rbtree
			 * @brief Red Black Tree
			 */
			adt::RBTree<Key, Compare> rbtree;

		public:
			/**
			 * @struct iterator
			 * @brief Iterator for Node
			 */
			struct iterator {
					/**
					 * @var node
					 * @brief Underlying node
					 */
					Node* node;

					/**
					 * @fn iterator(Node* node)
					 * @brief Create iterator
					 */
					iterator(Node* node) : node(node) {}

					/**
					 * @fn iterator& operator++()
					 * @brief Increment interator
					 */
					iterator& operator++() {
						node = node->next();
						return *this;
					}

					/**
					 * @fn iterator& operator++(int)
					 * @brief Increment interator
					 */
					iterator& operator++(int) {
						iterator ret(*this);
						node = node->next();
						return ret;
					}

					/**
					 * @fn bool operator==(const iterator& other) const
					 * @brief Equal compare
					 */
					bool operator==(const iterator& other) const {
						return node == other.node;
					}

					/**
					 * @fn bool operator!=(const iterator& other) const
					 * @brief Non-Equal compare
					 */
					bool operator!=(const iterator& other) const {
						return node != other.node;
					}

					/**
					 * @fn Key& operator*()
					 * @brief Accesss underlying Key
					 */
					Key& operator*() {
						return node->t;
					}

					/**
					 * @fn const Key& operator*()
					 * @brief Accesss underlying Key
					 */
					const Key& operator*() const {
						return node->t;
					}

			};

			/**
			 * @typedef key_type
			 * @brief Key type
			 */
			using key_type = Key;

			/**
			 * @typedef value_type
			 * @brief Value type
			 */
			using value_type = Key;

			/**
			 * @typedef size_type
			 * @brief Size type
			 */
			using size_type	= size_t;

			/**
			 * @typedef key_compare
			 * @brief Compare function
			 */
			using key_compare = Compare;

			/**
			 * @typedef value_compare
			 * @brief Compare function
			 */
			using value_compare = Compare;

			/**
			 * @typedef reference
			 * @brief Reference type
			 */
			using reference	= value_type&;

			/**
			 * @typedef const_reference
			 * @brief Constant reference type
			 */
			using const_reference = const value_type&;

			/**
			 * @typedef pointer
			 * @brief Pointeriterator erase( const_iterator pos );
(since C++11)
iterator erase( iterator pos );
			 */
			using pointer = Key*;

			/**
			 * @typedef const_pointer
			 * @brief Constant Pointer
			 */
			using const_pointer = const pointer;

			/**
			 * @fn set()
			 * @brief Create empty set
			 */
			set() {}

			set(const set& other) = delete;

			/**
			 * @fn set(set&& other)
			 * @brief Move Construct
			 */
			set(set&& other) {
				rbtree(other.rbtree);
			}

			/**
			 * @fn ~set
			 * @brief Destruct set
			 */
			~set() {
				clear();
			}

			set& operator=(const set& other) = delete;

			/**
			 * @fn set& operator=(set&& other)
			 * @brief Move Assignment
			 */
			set& operator=(set&& other) {
				rbtree = other.rbtree;
			}

			/**
			 * @fn bool empty() const
			 * @brief Check if set is empty
			 */
			bool empty() const {
				return rbtree.empty();
			}

			/**
			 * @fn size_t size() const
			 * @brief Get number of entries in set
			 */
			size_t size() const {
				return rbtree.size();
			}

			/**
			 * @fn void clear()
			 * @brief Erases all elements from the container
			 */
			void clear() {
				auto numEntries = rbtree.size();
				for (size_t i = 0; i < numEntries; i++) {
					auto node = rbtree.first();
					assert(node != nullptr);
					rbtree.remove(node);
					delete node;
				}
			}

			/**
			 * @fn int insert(const value_type& value)
			 * @brief Insert value if non existing
			 * @return
			 *
			 *	-  0      - Succcess
			 *	- -EINVAL - Value already exists
			 *	- -ENOMEM - Failed allocation
			 */
			int insert(const value_type& value) {
				/* Check if entry already exists */
				auto entry = rbtree.search(value);
				if (entry != nullptr)
					return -EINVAL;

				/* Allocate new entry */
				entry = new Node(value);
				if (entry == nullptr)
					return -ENOMEM;

				/* Insert into tree */
				rbtree.insert(entry);
				return 0;
			}

			/**
			 * @fn int insert(value_type&& value)
			 * @brief Insert value if non existing
			 * @return
			 *
			 *	-  0      - Succcess
			 *	- -EINVAL - Value already exists
			 *	- -ENOMEM - Failed allocation
			 */
			int insert(value_type&& value) {
				/* Check if entry already exists */
				auto entry = rbtree.search(value);
				if (entry != nullptr)
					return -EINVAL;

				/* Allocate new entry */
				entry = new Node(lib::move(value));
				if (entry == nullptr)
					return -ENOMEM;

				/* Insert into tree */
				rbtree.insert(entry);
				return 0;
			}

			/**
			 * @fn int emplace(Args&&... args)
			 * @brief Empace value if non existing
			 */
			template<typename... Args>
			int emplace(Args&&... args) {
				/* Allocate new entry */
				auto entry = new Node(lib::forward<Args>(args)...);
				if (entry == nullptr)
					return -ENOMEM;

				if (rbtree.search(entry->t) != nullptr) {
					delete entry;
					return -EINVAL;
				}

				/* Insert into tree */
				rbtree.insert(entry);
				return 0;
			}

			/**
			 * @fn void erase(iterator pos)
			 * @brief Erase entry for given position
			 */
			void erase(iterator pos) {
				auto entry = pos.node;
				rbtree.remove(entry);
				delete entry;
			}

			/**
			 * @fn size_type erase(const key_type& key)
			 * @brief Erase entry for given key
			 * @return
			 *
			 *	- 0 - No entry removed
			 *	- 1 - Entry removed
			 */
			size_type erase(const key_type& key) {
				auto entry = rbtree.search(key);
				if (entry == nullptr)
					return 0;

				rbtree.remove(entry);
				delete entry;
				return 1;
			}

			/**
			 * @fn iterator find(const Key& key)
			 * @brief Find entry for given key
			 */
			iterator find(const Key& key) {
				auto entry = rbtree.search(key);
				return iterator(entry);
			}

			/**
			 * @fn iterator begin()
			 * @brief Return iterator to start
			 */
			iterator begin() {
				return iterator(rbtree.first());
			}

			/**
			 * @fn iterator end()
			 * @brief Return iterator to end
			 */
			iterator end() {
				return iterator(nullptr);
			}

	};
}

#endif /* ifndef _INC_SET_H_ */
