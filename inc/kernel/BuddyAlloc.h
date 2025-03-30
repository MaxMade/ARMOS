#ifndef _INC_BUDDY_ALLOC_H_
#define _INC_BUDDY_ALLOC_H_

#include <cstddef.h> /* size_t */
#include <cstring.h> /* memset */
#include <cstdint.h> /* uintptr_t */

#include <kernel/math.h>

/**
 * @file BuddyAlloc.h
 * @brief Generic Buddy Allocator
 */

/* XXX: Comment in for debug assertions */
//#define BUDDY_ALLOC_ASSERT 1

/* Setting for assertions */
#ifdef BUDDY_ALLOC_ASSERT
#include <cassert> /* assert */
#define buddy_assert(x) assert(x)
#else
#define buddy_assert(x)
#endif /* ifdef BUDDY_ALLOC_DEBUG */

#ifndef container_of
#define container_of(ptr, type, member) ((type *) (((char *) ptr) - offsetof(type,member)))
#endif /* container_of */

/**
 * @class BuddyAllocator
 * @brief Buddy Allocator
 * @tparam MIN_SIZE Minimum allocation size
 * @tparam MAX_SIZE Maximum allocation size
 * @warning MIN_SIZE and MAX_SIZE must be power of two!
 */
template<size_t MIN_SIZE, size_t MAX_SIZE>
class BuddyAllocator {
	private:

		/**
		 * @fn static constexpr int ilog2(size_t val)
		 * @brief Compute floor of binary logarithm of val
		 */
		static constexpr int ilog2(size_t val) {
			buddy_assert(val != 0);
			return 8 * sizeof(val) - __builtin_clzll(val) - 1;
		}

		/**
		 * @fn static constexpr size_t roundUpPowerTwo(size_t val)
		 * @brief Round up to the next power of two if necessary
		 */
		static constexpr size_t roundUpPowerTwo(size_t val) {
			/* Check if already power of two */
			if (math::isPowerOfTwo(val)) {
				return val;
			}

			/* Handle corner case */
			if (val == 0) {
				return 1;
			}
			
			return 1 << (ilog2(val) + 1);
		}

		/**
		 * @fn static constexpr size_t roundDownPowerTwo(size_t val)
		 * @brief Round up to the next power of two if necessary
		 */
		static constexpr size_t roundDownPowerTwo(size_t val) {
			/* Check if already power of two */
			if (math::isPowerOfTwo(val)) {
				return val;
			}

			/* Handle corner case */
			if (val == 0) {
				return 0;
			}
			
			return 1 << ilog2(val);
		}


		/**
		 * @fn size_t sizeToIndex(size_t size)
		 * @brief Convert size to index in \c buddies
		 */
		size_t sizeToIndex(size_t size) {
			buddy_assert(size >= MIN_SIZE);
			buddy_assert(size <= MAX_SIZE);
			buddy_assert(math::isPowerOfTwo(size));

			size_t idx = ilog2(size) - ilog2(MIN_SIZE);
			buddy_assert(idx < NUM_BUDDIES);

			return idx;
		}

		/**
		 * @struct UsedMemory
		 * @brief Struct to represent used memory
		 */
		struct UsedMemory {
			bool used;   /**< Flag to indicate used memory (should be true) */
			size_t size; /**< Total size of memory region */
			char mem[0]; /**< Pointer to useable memory */
		};

		/**
		 * @struct FreeMemory
		 * @brief Struct to represent free memory
		 */
		struct FreeMemory {
			bool used;        /**< Flag to indicate used memory (should be false) */
			size_t size;      /**< Size */
			FreeMemory* prev; /**< Previous Link in Linked List */
			FreeMemory* next; /**< Next Link in Linked List */
		};

		/**
		 * @var NUM_BUDDIES
		 * @breif Number of entries in \c buddies
		 */
		static const size_t NUM_BUDDIES = ilog2(MAX_SIZE) - ilog2(MIN_SIZE) + 1;

		/**
		 * @var buddies
		 * @brief Array of buddy lists
		 */
		FreeMemory* buddies[NUM_BUDDIES] = {nullptr};

		/**
		 * @fn bool checkBuddy(FreeMemory* buddy)
		 * @brief
		 */
		bool checkBuddy(FreeMemory* buddy) {
			if (reinterpret_cast<uintptr_t>(buddy) < reinterpret_cast<uintptr_t>(__mem))
				return false;

			if (reinterpret_cast<uintptr_t>(buddy) >= reinterpret_cast<uintptr_t>(__mem) + __size)
				return false;

			if (buddy->size < MIN_SIZE)
				return false;

			if (buddy->size > MAX_SIZE)
				return false;

			if (!math::isPowerOfTwo(buddy->size))
				return false;

			return true;
		}

		/**
		 * @fn FreeMemory* findBuddy(FreeMemory* buddy)
		 * @brief Find associated buddy for given \c buddy and \c size
		 */
		FreeMemory* findBuddy(FreeMemory* buddy) {
			buddy_assert(checkBuddy(buddy));
			auto size = buddy->size;

			auto offset = reinterpret_cast<uintptr_t>(buddy) - reinterpret_cast<uintptr_t>(__mem);
			offset = offset ^ size;

			auto ret = reinterpret_cast<FreeMemory*>(reinterpret_cast<uintptr_t>(__mem) + offset);

			if (ret->used || ret->size != size ||
					reinterpret_cast<uintptr_t>(ret) < reinterpret_cast<uintptr_t>(__mem) ||
					reinterpret_cast<uintptr_t>(ret) >= reinterpret_cast<uintptr_t>(__mem) + __size) {

				return nullptr;
			}

			return ret;
		}

		/**
		 * @fn void insertFreeMemory(FreeMemory* entry)
		 * @brief Prepend \c entry in \c list
		 */
		void insertFreeMemory(FreeMemory* entry) {
			buddy_assert(entry);

			entry->next = nullptr;
			entry->prev = nullptr;
			entry->used = false;

			auto idx = sizeToIndex(entry->size);

			if (buddies[idx] == nullptr) {
				buddies[idx] = entry;
			} else {
				auto oldHead = buddies[idx];
				oldHead->prev = entry;
				entry->next = oldHead;
				buddies[idx] = entry;
			}

		}

		/**
		 * @fn FreeMemory* removeFreeMemory(FreeMemory* entry)
		 * @brief Remove \c entry from \c list
		 * @return \c entry
		 */
		FreeMemory* removeFreeMemory(FreeMemory* entry) {
			buddy_assert(entry);

			/* Find assocated list */
			auto idx = sizeToIndex(entry->size);
			buddy_assert(buddies[idx] != nullptr);

			/* Save next and previous pointer */
			auto prev = entry->prev;
			auto next = entry->next;

			/* Update next */
			if (next != nullptr) {
				buddy_assert(checkBuddy(next));
				next->prev = prev;
			}

			/* Update prev */
			if (prev != nullptr) {
				buddy_assert(checkBuddy(prev));
				prev->next = next;

			/* If no prev, entry was old head -> next is new head */
			} else {
				buddies[idx] = next;
			}

			return entry;
		}

		/**
		 * @fn void split(size_t idx)
		 * @brief Split head of buddies[idx] and update buddies
		 */
		void split(size_t idx) {
			buddy_assert(idx > 0);
			buddy_assert(idx < NUM_BUDDIES);
			buddy_assert(buddies[idx] != nullptr);

			/* Get left split */
			auto left = removeFreeMemory(buddies[idx]);
			buddy_assert(checkBuddy(left));
			auto size = left->size;

			/* Get right split */
			auto right = reinterpret_cast<FreeMemory*>(reinterpret_cast<uintptr_t>(left) + size / 2);
			buddy_assert(checkBuddy(left));

			/* Update size */
			left->size = size / 2;
			right->size = size / 2;

			/* Re-insert */
			insertFreeMemory(right);
			insertFreeMemory(left);

		}

		/**
		 * @fn FreeMemory* tryMerge(FreeMemory* mem)
		 * @brief Try to merge FreeMemory entries
		 * @warning mem must be part of an list in buddies!
		 * @return
		 *
		 *	- Pointer to merged (and re-inserted) FreeMemory - Success
		 *	- nullptr - Failure
		 */
		FreeMemory* tryMerge(FreeMemory* mem) {
			buddy_assert(checkBuddy(mem));
			buddy_assert(mem->used == false);

			/* Find buddy */
			auto buddy = findBuddy(mem);
			if (buddy == nullptr) {
				return nullptr;
			}
			buddy_assert(checkBuddy(buddy));
			buddy_assert(mem->size == buddy->size);

			/* Remove buddies */
			removeFreeMemory(mem);
			removeFreeMemory(buddy);

			/* Re-insert */
			auto newMem = reinterpret_cast<uintptr_t>(mem) < reinterpret_cast<uintptr_t>(buddy) ? mem : buddy;
			newMem->size = 2 * newMem->size;
			buddy_assert(checkBuddy(newMem));
			insertFreeMemory(newMem);

			return newMem;
		}

		/**
		 * @var __mem
		 * @brief Underlying memory
		 */
		void* __mem = nullptr;
		/**
		 * @var __size
		 * @brief Size of mem
		 */
		size_t __size = 0;

		static_assert(offsetof(UsedMemory, used) == offsetof(FreeMemory, used),
				"Member 'used' must be at the same offset within UsedMemory and FreeMemory!");

		static_assert(offsetof(UsedMemory, size) == offsetof(FreeMemory, size),
				"Member 'size' must be at the same offset within UsedMemory and FreeMemory!");

		static_assert(MIN_SIZE >= sizeof(FreeMemory),
				"MIN_SIZE must be at least sizeof(FreeMemory)!");

		static_assert(__builtin_popcountll(MIN_SIZE) == 1, "MIN_SIZE must be power of two");

		static_assert(__builtin_popcountll(MAX_SIZE) == 1, "MAX_SIZE must be power of two");

	public:
		/**
		 * @fn constexpr BuddyAllocator(void* mem = nullptr, size_t size = 0)
		 * @brief Innitialize Allocator
		 * @param mem Pointer to memory
		 * @param size Size of memory
		 * @warning size must be at lesast MIN_SIZE and must not be larger than MAX_SIZE!
		 */
		constexpr BuddyAllocator(void* mem = nullptr, size_t size = 0) {
			/* Checks of size */
			if (size < MIN_SIZE) {
				return;
			}

			if (size > MAX_SIZE) {
				size = MAX_SIZE;
			}

			/* Save mem and size */
			__mem = mem;
			__size = size;

			/* Create initial FreeMemory */
			auto entry = reinterpret_cast<FreeMemory*>(mem);
			lib::memset(entry, 0, sizeof(*entry));
			entry->size = 1 << ilog2(size);

			/* Insert into list */
			auto idx = ilog2(size) - ilog2(MIN_SIZE);
			buddies[idx] = entry;
		}

		BuddyAllocator(const BuddyAllocator& o) = delete;

		BuddyAllocator(BuddyAllocator&& o) = delete;

		/**
		 * @fn void* allocate(size_t size)
		 * @brief Allocte \c size bytes
		 */
		void* allocate(size_t size) {
			void *ret = nullptr;

			/* Early out: Tried to allocate zero bytes */
			if (size == 0) {
				return nullptr;
			}

			/* Calculate actual needed size */
			auto oldSize = size;
			(void) oldSize;
			size += sizeof(UsedMemory);
			size = roundUpPowerTwo(size);

			/* Early out: Exceeded MAX_SIZE */
			if (size > MAX_SIZE) {
				return nullptr;
			}

			/* Search for memory and calculate number of splits */
			int numSplits = -1;
			auto fittingIdx = sizeToIndex(size);
			for (size_t i = fittingIdx; i < NUM_BUDDIES; i++) {
				if (buddies[i] != nullptr) {
					numSplits = i - fittingIdx;
					break;
				}
			}

			/* Check if enough memory */
			if (numSplits == -1) {
				return nullptr;
			}

			/* Perform splits */
			buddy_assert(buddies[fittingIdx + numSplits] != nullptr);
			for (int i = 0; i < numSplits; i++) {
				split(fittingIdx + numSplits - i);
			}

			/* Perfom actual allocation */
			buddy_assert(buddies[fittingIdx] != nullptr);
			auto freeMem = removeFreeMemory(buddies[fittingIdx]);
			buddy_assert(freeMem->used == false);
			auto usedMem = reinterpret_cast<UsedMemory*>(freeMem);
			usedMem->used = true;
			buddy_assert(usedMem->size == size);
			ret = usedMem->mem;

			return ret;
		}

		void deallocate(void* ptr) {
			/* Early out for NULL value */
			if (ptr == nullptr) {
				return;
			}

			/* Check pointer */
			if (reinterpret_cast<uintptr_t>(ptr) < reinterpret_cast<uintptr_t>(__mem)) {
				buddy_assert(reinterpret_cast<uintptr_t>(ptr) < reinterpret_cast<uintptr_t>(__mem));
				return;
			}
			if (reinterpret_cast<uintptr_t>(ptr) >= reinterpret_cast<uintptr_t>(__mem) + __size) {
				buddy_assert(reinterpret_cast<uintptr_t>(ptr) >= reinterpret_cast<uintptr_t>(__mem) + __size);
				return;
			}

			/* Cast to UsedMemory */
			auto usedMem = container_of(ptr, UsedMemory, mem);
			if (usedMem->used == false) {
				buddy_assert(usedMem->used == false);
				return;
			}
			auto size = usedMem->size;

			/* Cast to FreeMemory */
			auto mem = reinterpret_cast<FreeMemory*>(usedMem);
			lib::memset(mem, 0, sizeof(*mem));
			mem->size = size;
			buddy_assert(checkBuddy(mem));
			insertFreeMemory(mem);

			/* Try to merge */
			do {
				mem = tryMerge(mem);
			} while (mem != nullptr);
		}
};

#endif /* ifndef _INC_BUDDY_ALLOC_H_ */
