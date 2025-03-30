#ifndef _INC_KERNEL_LOCK_SPINLOCK_H_
#define _INC_KERNEL_LOCK_SPINLOCK_H_

#include <atomic.h>

/**
 * @file kernel/lock/spinlock.h
 * @brief Spinlock
 */

namespace lock {

	/**
	 * @class spinlock
	 * @brief Spinlock
	 */
	class spinlock {
		private:
			/**
			 * @var lock_flag
			 * @brief Underlying lock flag
			 */
			lib::atomic_flag lock_flag;

		public:
			/**
			 * @fn spinlock
			 * @brief Construct unlocked spinlock
			 */
			spinlock();

			spinlock(const spinlock&) = delete;

			spinlock(spinlock&&) = delete;

			/**
			 * @fn void lock()
			 * @brief Lock the spinlock
			 */
			void lock();

			/**
			 * @fn bool tryLock()
			 * @brief Try to lock the spinlock
			 * @return
			 *
			 *	- true  - Locking attempt was successfull
			 *	- false - Unable to lock
			 */
			bool tryLock();

			/**
			 * @fn void unlock()
			 * @brief Unlock the spinlock
			 */
			void unlock();
	};

} /* namespace lock */

#endif /* ifndef _INC_KERNEL_LOCK_SPINLOCK_H_ */
