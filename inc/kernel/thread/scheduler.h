#ifndef _INC_KERNEL_THREAD_SCHEDULER_H_
#define _INC_KERNEL_THREAD_SCHEDULER_H_

#include <map.h>
#include <list.h>
#include <memory.h>
#include <algorithm.h>
#include <kernel/cpu_local.h>
#include <kernel/lock/spinlock.h>
#include <kernel/thread/context.h>

namespace thread {

	class Scheduler {
		private:
			size_t nextUsedID;

			/**
			 * @var queue
			 * @brief Scheduler Queue
			 */
			lib::list<lib::shared_ptr<Context>> queue;

			/**
			 * @var threads
			 * @brief Map of managed IDs and threads
			 */
			lib::map<size_t, lib::shared_ptr<Context>, lib::greater<size_t>> threads;

			/**
			 * @var activeThreads
			 * @brief Currently active threads
			 */
			cpu_local<lib::shared_ptr<Context>> activeThreads;

			/**
			 * @var lock
			 * @brief Synchronization lock
			 */
			lock::spinlock lock;

			/**
			 * @fn void __schedule(lock::spinlock* lock)
			 * @brief Internal schedule call
			 */
			void __schedule(lock::spinlock* lock);

		public:
			/**
			 * @fn Scheduler()
			 * @brief Constructor
			 */
			Scheduler();

			Scheduler(const Scheduler& o) = delete;

			Scheduler(Scheduler&& o) = delete;

			/**
			 * @fn int create(void* (*start_routine)(void*), void* arg, void *user_stack)
			 * @brief Create new thread with given \c start_routine and \c arg using \c user_stack
			 * @return
			 *
			 *	-  >=0 - Success (Thread ID)
			 *	-   <0 - Failure (-errno)
			 */
			int create(void* (*start_routine)(void*), void* arg, void *user_stack);

			/**
			 * @fn lib::shared_ptr<Context> getActive()
			 * @brief Get currently active thread
			 * @warning In case of idlethreads, static_cast<bool>(getActive) will evaluate to false
			 */
			lib::shared_ptr<Context> getActive();

			/**
			 * @fn lib::shared_ptr<Context> getThread(size_t id)
			 * @brief Get thread for given ID
			 * @warning Returned shared pointer might be invalid
			 */
			lib::shared_ptr<Context> getThread(size_t id);

			/**
			 * @fn void schedule()
			 * @brief Initiate context switch
			 */
			void schedule();

			/**
			 * @fn void schedule(lock::spinlock& lock)
			 * @brief Initiate context switch and unlock spinlock
			 */
			void schedule(lock::spinlock& lock);

			/**
			 * @fn void __unlock()
			 * @brief Manually unlock scheduler
			 * @warning This function mustn't be called directly
			 */
			void __unlock();
	};

	/**
	 * @var scheduler
	 * @brief Global Scheduler Object
	 */
	extern Scheduler scheduler;

} /* namespace thread */

#endif /* ifndef _INC_KERNEL_THREAD_SCHEDULER_H_ */
