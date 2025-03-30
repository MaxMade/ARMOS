#ifndef _INC_KERNEL_MM_FRAME_ALLOCATOR_H_
#define _INC_KERNEL_MM_FRAME_ALLOCATOR_H_

#include <cstddef.h>
#include <climits.h>
#include <kernel/lock/spinlock.h>

/**
 * @file kernel/mm/frame_allocator.h
 * @brief Allocate page frames
 */

namespace mm {

	/**
	 * @class FrameAllocator
	 * @brief Frame Allocator
	 */
	class FrameAllocator {
		private:
			/**
			 * @var NUM_FRAMES
			 * @brief Number of pages within allocator
			 */
			static const size_t NUM_FRAMES = 4096;

			/**
			 * @var SIZE
			 * @brief Size of allocator in bytes
			 */
			static const size_t SIZE = NUM_FRAMES * PAGESIZE;
			char frames[SIZE] __attribute__((aligned(PAGESIZE)));

			/**
			 * @struct FrameLink
			 * @brief Link between available page frames
			 */
			struct FrameLink {
				FrameLink* next;
			};

			/**
			 * @var head
			 * @brief Pointer to top of stack
			 */
			FrameLink* head = nullptr;

			/**
			 * @var lock
			 * @brief Synchronization lock
			 */
			lock::spinlock lock;

		public:
			/**
			 * @fn FrameAllocator
			 * @brief Initialize allocator
			 */
			FrameAllocator();

			FrameAllocator(const FrameAllocator& other) = delete;

			FrameAllocator(FrameAllocator&& other) = delete;

			FrameAllocator& operator=(const FrameAllocator& other) = delete;

			FrameAllocator& operator=(FrameAllocator&& other) = delete;

			/**
			 * @fn void init()
			 * @brief Initialize allocator
			 */
			void init();

			/**
			 * @fn void* earlyAlloc()
			 * @brief Allocate page frame without locking
			 * @warning This function should be only used for the early boot process
			 * @return
			 *
			 *	- Pointer to pages - Success
			 *	- nullptr          - Failure
			 */
			void* earlyAlloc();

			/**
			 * @fn int earlyFree(void *page)
			 * @brief Free page frame without locking
			 * @warning This function should be only used for the early boot process
			 * @return
			 *
			 *	- 0  - Success
			 *	- <0 - Failure (-errno)
			 */
			int earlyFree(void *page);

			/**
			 * @fn void* alloc()
			 * @brief Allocate page frame
			 * @return
			 *
			 *	- Pointer to pages - Success
			 *	- nullptr          - Failure
			 */
			void* alloc();

			/**
			 * @fn int free(void *page)
			 * @brief Free page frame
			 * @return
			 *
			 *	- 0  - Success
			 *	- <0 - Failure (-errno)
			 */
			int free(void *page);

	};

	extern FrameAllocator frameAlloc;

} /* namespace mm */

#endif /* ifndef _INC_KERNEL_MM_FRAME_ALLOCATOR_H_ */
