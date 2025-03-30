#ifndef _INC_KERNEL_MM_TRANSLATION_TABLE_ALLOCATOR_H_
#define _INC_KERNEL_MM_TRANSLATION_TABLE_ALLOCATOR_H_

#include <cstddef.h>
#include <climits.h>
#include <kernel/lock/spinlock.h>

/**
 * @file kernel/mm/translation_table_allocator.h
 * @brief Allocate pages for translation tables
 */

namespace mm {

	/**
	 * @class TTAllocator
	 * @brief Translation Table Allocator
	 */
	class TTAllocator {
		private:
			/**
			 * @var NUM_PAGES
			 * @brief Number of pages within allocator
			 */
			static const size_t NUM_PAGES = 4096;

			/**
			 * @var SIZE
			 * @brief Size of allocator in bytes
			 */
			static const size_t SIZE = NUM_PAGES * PAGESIZE;
			char pages[SIZE] __attribute__((aligned(PAGESIZE)));

			/**
			 * @struct TTPageLink
			 * @brief Link used for linking available pages
			 */
			struct TTPageLink {
				TTPageLink* next;
			};

			/**
			 * @var head
			 * @brief Pointer to top of stack
			 */
			TTPageLink* head = nullptr;

			/**
			 * @var lock
			 * @brief Synchronization lock
			 */
			lock::spinlock lock;

		public:
			/**
			 * @fn TTAllocator
			 * @brief Initialize allocator
			 */
			TTAllocator();

			TTAllocator(const TTAllocator& other) = delete;

			TTAllocator(TTAllocator&& other) = delete;

			TTAllocator& operator=(const TTAllocator& other) = delete;

			TTAllocator& operator=(TTAllocator&& other) = delete;

			/**
			 * @fn void init()
			 * @brief Initialize allocator
			 */
			void init();

			/**
			 * @fn void* earlyAlloc()
			 * @brief Allocate translation table without locking
			 * @warning This function should be only used for the early boot process
			 * @return
			 *
			 *	- Pointer to pages - Success
			 *	- nullptr          - Failure
			 */
			void* earlyAlloc();

			/**
			 * @fn int earlyFree(void *page)
			 * @brief Free translation table without locking
			 * @warning This function should be only used for the early boot process
			 * @return
			 *
			 *	- 0  - Success
			 *	- <0 - Failure (-errno)
			 */
			int earlyFree(void *page);

			/**
			 * @fn void* alloc()
			 * @brief Allocate translation table
			 * @return
			 *
			 *	- Pointer to pages - Success
			 *	- nullptr          - Failure
			 */
			void* alloc();

			/**
			 * @fn int free(void *page)
			 * @brief Free translation table
			 * @return
			 *
			 *	- 0  - Success
			 *	- <0 - Failure (-errno)
			 */
			int free(void *page);

	};

	extern TTAllocator ttAlloc;

} /* namespace mm */

#endif /* ifndef _INC_KERNEL_MM_TRANSLATION_TABLE_ALLOCATOR_H_ */
