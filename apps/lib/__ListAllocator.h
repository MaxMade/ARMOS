#ifndef _INC_LIST_ALLOCATOR_H_
#define _INC_LIST_ALLOCATOR_H_

#include <stddef.h> /* size_t */
#include <string.h> /* memset */
#include <stdint.h> /* uinptr_t */

// #define LIST_ALLOC_ASSERT 1

/* Setting for assertions */
#ifdef LIST_ALLOC_ASSERT
#include <assert.h> /* assert */
#define list_assert(x) assert(x)
#else
#define list_assert(x)
#endif /* ifdef LIST_ALLOC_DEBUG */

#ifndef container_of
#define container_of(ptr, type, member) ((type *) (((char *) ptr) - offsetof(type,member)))
#endif /* container_of */

/**
 * @file apps/lib/__ListAllocator.h
 * @brief List-Based Allocator
 */

/**
 * @enum SearchPolicy
 * @brief Allocator Policy
 */
enum SearchPolicy {
	FirstFit, /**< Use first fitting memory region */
	BestFit,  /**< Use best fitting memory region */
	WorstFit, /**< Use worst fitting memory region */
};

/**
 * @class ListAllocator
 * @brief List-Based Allocator
 * @tparam MIN_ALLOC Minimum Allocation Size
 * @tparam POLICY Allocator Policy
 */
template<size_t MIN_ALLOC, SearchPolicy POLICY>
class ListAllocator {
	private:

		/**
		 * @struct ListEntry
		 * @brief List entry
		 */
		struct ListEntry {
			size_t size;     /**< Size of memory region */
			ListEntry* next; /**< Next pointer */
			ListEntry* prev; /**< Previous pointer */
		};

		/**
		 * @struct AllocatedRegion 
		 * @brief Allocated memory region
		 */
		struct AllocatedRegion {
			size_t size; /**< Size of memory region */
			char mem[0]; /**< Pointer to memory */
		};

		/**
		 * @var head
		 * @brief Head of list
		 */
		ListEntry* head;

		/**
		 * @fn void insert(ListEntry* entry)
		 * @brief Insert \c entry into list
		 */
		void insert(ListEntry* entry) {
			entry->next = head;
			entry->prev = nullptr;

			if (head != nullptr) {
				head->prev = entry;
			}

			head = entry;
		}


		/**
		 * @fn void remove(ListEntry* entry)
		 * @brief Remove \c entry from list
		 */
		void remove(ListEntry* entry) {
			auto prev = entry->prev;
			auto next = entry->next;

			if (next != nullptr) {
				next->prev = prev;
			}

			if (prev != nullptr) {
				prev->next = next;
			} else {
				head = next;
			}
		}

		/**
		 * @fn void findAdjacent(ListEntry* entry, ListEntry** prev, ListEntry** next)
		 * @breif Find adjacent entries if possible
		 */
		void findAdjacent(ListEntry* entry, ListEntry** prev, ListEntry** next) {
			*prev = nullptr;
			*next = nullptr;

			for (auto drag = head; drag != nullptr; drag = drag->next) {
				/* Check if next */
				if (reinterpret_cast<uintptr_t>(entry) + entry->size == reinterpret_cast<uintptr_t>(drag)) {
					list_assert(*next == nullptr);					
					*next = drag;
				}

				/* Check if prev */
				if (reinterpret_cast<uintptr_t>(drag) + drag->size == reinterpret_cast<uintptr_t>(entry)) {
					list_assert(*prev == nullptr);					
					*prev = drag;
				}

				if (*prev != nullptr && *next != nullptr) {
					break;
				}
			}
		}

		/**
		 * @fn inline ListEntry* findFirstFit(size_t size)
		 * @brief Find entry based on FirstFit Policy
		 */
		inline ListEntry* findFirstFit(size_t size) {
			for (auto drag = head; drag != nullptr; drag = drag->next) {
				if (drag->size >= size) {
					return drag;
				}
			}

			return nullptr;
		}

		/**
		 * @fn inline ListEntry* findBestFit(size_t size)
		 * @brief Find entry based on BestFit Policy
		 */
		inline ListEntry* findBestFit(size_t size) {
			ListEntry* ret = nullptr;
			size_t diff = -1;

			for (auto drag = head; drag != nullptr; drag = drag->next) {
				if (drag->size >= size) {
					auto localDiff = drag->size - size;
					if (localDiff < diff) {
						diff = localDiff;
						ret = drag;
					}
				}
			}

			return ret;
		}

		/**
		 * @fn inline ListEntry* findWorstFit(size_t size)
		 * @brief Find entry based on WorstFit Policy
		 */
		inline ListEntry* findWorstFit(size_t size) {
			ListEntry* ret = nullptr;
			size_t diff = 0;

			for (auto drag = head; drag != nullptr; drag = drag->next) {
				if (drag->size >= size) {
					auto localDiff = drag->size - size;
					if (localDiff > diff) {
						diff = localDiff;
						ret = drag;
					}
				}
			}

			return ret;
		}

		/**
		 * @fn ListEntry* find(size_t size)
		 * @brief Wrapper for find functions
		 */
		ListEntry* find(size_t size) {
			if constexpr (POLICY == FirstFit) {
				return findFirstFit(size);
			}

			if constexpr (POLICY == BestFit) {
				return findBestFit(size);
			}

			if constexpr (POLICY == WorstFit) {
				return findWorstFit(size);
			}
		}

		static_assert(sizeof(ListEntry) <= MIN_ALLOC, "MIN_ALLOC must be at least sizeof(ListEntry) bytes!");
		static_assert(POLICY == FirstFit || POLICY == BestFit || POLICY == WorstFit, "POLICY must be either FirstFit, BestFit or WorstFit");

	public:
		/**
		 * @fn ListAllocator(void* mem, size_t size)
		 * @brief Fill ListAllocator initally
		 */
		ListAllocator(void* mem, size_t size) : head(nullptr) {
			/* Round down to multiple of MIN_ALLOC */
			auto reminder = size % MIN_ALLOC;
			size -= reminder;

			/* Check if at least MIN_ALLC */
			if (size < MIN_ALLOC) {
				return;
			}

			/* Prepare entry */
			auto entry = reinterpret_cast<ListEntry*>(mem);
			*entry = {
				.size = size,
				.next = nullptr,
				.prev = nullptr,
			};

			/* Insert into list */
			head = entry;
		}

		/**
		 * @fn void* allocate(size_t size)
		 * @brief Allocate \c size bytes
		 */
		void* allocate(size_t size) {
			/* Sanity check */
			if (size == 0) {
				return nullptr;
			}

			/* Add size of header */
			size = size + sizeof(AllocatedRegion);

			/* Round up to multiple of MIN_ALLOC */
			if (auto reminder = size % MIN_ALLOC; reminder != 0) {
				size += MIN_ALLOC - reminder;
			}

			/* Find suitable node and remove it */
			ListEntry* entry = find(size);
			if (entry == nullptr) {
				return nullptr;
			}
			remove(entry);

			/* Reinsert if necessary */
			if (entry->size > size) {
				auto oldSize = entry->size;
				auto node = reinterpret_cast<ListEntry*>(reinterpret_cast<uintptr_t>(entry) + size);
				node->size = oldSize - size;
				list_assert(node->size % MIN_ALLOC == 0);
				insert(node);
			}
			
			
			auto ret = reinterpret_cast<AllocatedRegion*>(entry);
			ret->size = size;
			return ret->mem;
		}

		/**
		 * @fn void deallocate(void *ptr)
		 * @brief Free \c ptr
		 */
		void deallocate(void *ptr) {
			if (ptr == nullptr) {
				return;
			}

			auto used = container_of(ptr, AllocatedRegion, mem);
			list_assert(used->size % MIN_ALLOC == 0);
			size_t size = used->size;

			auto entry = reinterpret_cast<ListEntry*>(used);
			entry->size = size;

			ListEntry* prev = nullptr;
			ListEntry* next = nullptr;
			findAdjacent(entry, &prev, &next);

			if (next != nullptr) {
				remove(next);
				entry->size += next->size;
			}

			if (prev != nullptr) {
				remove(prev);
				prev->size += entry->size;
				entry = prev;
			}
			
			insert(entry);
		}

};

#endif /* ifndef _INC_LIST_ALLOCATOR_H_ */
