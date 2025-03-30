#ifndef _INC_KERNEL_LOCK_SEMAPHORE_H_
#define _INC_KERNEL_LOCK_SEMAPHORE_H_

#include <list.h>
#include <memory.h>
#include <kernel/lock/spinlock.h>
#include <kernel/thread/context.h>

/**
 * @file kernel/lock/semaphore.h
 * @brief Semaphore implementation
 */

namespace lock {

	/**
	 * @class Semaphore
	 * @brief Semaphore implementation
	 */
	class Semaphore {
		private:
			/**
			 * @var val
			 * @brief Current value
			 */
			int val;

			/**
			 * @var lock
			 * @brief Synchronazation lock
			 */
			lock::spinlock lock;

			/**
			 * @var queue
			 * @brief Local waiting queue
			 */
			lib::list<lib::shared_ptr<thread::Context>> queue;

		public:
			/**
			 * @fn Semaphore(int cnt)
			 * @brief Initialize semphore with count \c cnt
			 */
			explicit Semaphore(int cnt);

			/**
			 * @fn int p()
			 * @brief Decrement seamphore count and block eventually
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int p();

			/**
			 * @fn int v()
			 * @brief Increment seamphore count and unblock eventually
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int v();
	};

} /* namespace lock */

#endif /* ifndef _INC_KERNEL_LOCK_SEMAPHORE_H_ */
