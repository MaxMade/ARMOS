#ifndef _INC_MAP_H_
#define _INC_MAP_H_

#include <new.h>
#include <cerrno.h>
#include <utility.h>
#include <functional.h>
#include <kernel/adt/rbtree.h>

/**
 * @file map.h
 * @brief Sorted associative container that contains key-value pairs with unique keys
 */
namespace lib {

	/**
	 * @class map
	 * @brief Sorted associative container that contains key-value pairs with unique keys
	 */
	template <typename Key, typename T, typename Compare = lib::less<Key>>
	class map {
		private:
			/**
			 * @struct Entry
			 * @brief Entry in RB Tree
			 */
			struct Entry : public lib::pair<const Key, T> {

				/**
				 * @fn Entry()
				 * @brief Construct Entry
				 */
				Entry() : lib::pair<const Key, T>() {}

				/**
				 * @fn Entry(const lib::pair<const Key, T>& o)
				 * @brief Copy-Construct Entry from lib::pair
				 */
				Entry(const lib::pair<const Key, T>& o) : lib::pair<const Key, T>(o) { }

				/**
				 * @fn Entry(const lib::pair<const Key, T>& o)
				 * @brief Move-Construct Entry from lib::pair
				 */
				Entry(lib::pair<const Key, T>&& o) : lib::pair<const Key, T>(lib::move(o)) { }

				/**
				 * @fn bool operator==(const Entry& other) const
				 * @brief Compare operator
				 */
				bool operator==(const Entry& other) const {
					return this->first == other.first;
				}

				/**
				 * @fn bool operator!=(const Entry& other) const
				 * @brief Compare operator
				 */
				bool operator!=(const Entry& other) const {
					return this->first != other.first;
				}

				/**
				 * @fn bool operator<(const Entry& other) const
				 * @brief Compare operator
				 */
				bool operator<(const Entry& other) const {
					return this->first < other.first;
				}

				/**
				 * @fn bool operator>(const Entry& other) const
				 * @brief Compare operator
				 */
				bool operator>(const Entry& other) const {
					return this->first < other.k;
				}

			};

			/**
			 * @struct __Compare
			 * @brief Custom Compare Wrapper for RBTree
			 */
			struct __Compare {
				Compare cmp;

				bool operator()(const Entry& a, const Entry& b) {
					return cmp(a.first, b.first);
				};
			};

			/**
			 * @typedef Node
			 * @brief Alias for underlying RBNode
			 */
			using Node = typename adt::RBTree<Entry, __Compare>::RBNode;

			/**
			 * @var rbtree
			 * @brief Red Black Tree
			 */
			adt::RBTree<Entry, __Compare> rbtree;


		public:
			using key_type = Key;
			using mapped_type = T;
			using value_type = lib::pair<const Key, T>;
			using size_type	 = size_t;
			using key_compare = Compare;
			using reference	= value_type&;
			using const_reference = const value_type&;

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
					 * @fn value_type & operator*()
					 * @brief Accesss underlying Key
					 */
					value_type & operator*() {
						return node->t;
					}

					/**
					 * @fn const value_type & operator*() const
					 * @brief Accesss underlying Key
					 */
					const value_type & operator*() const {
						return node->t;
					}

			};

			map() { }

			map(const map& other) = delete;

			map(map&& other) = default;

			~map() {
				clear();
			}

			map& operator=(const map& other) = delete;

			map& operator=(map&& other) = default;

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
			 * @fn int emplace(O&& key, U&& t)
			 * @brief Emplace pair
			 */
			template<typename O, typename U>
			int emplace(O&& key, U&& t) {
				/* Allocate new entry */
				auto entry = new Node(lib::pair<const Key, T>(lib::forward<O>(key), lib::forward<U>(t)));
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
				Entry tmp(key, T());
				auto entry = rbtree.search(tmp);
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

} /* namespace lib */

#endif /* ifndef _INC_MAP_H_ */
