#ifndef _INC_KERNEL_THREAD_IDLE_H_
#define _INC_KERNEL_THREAD_IDLE_H_

/**
 * @file kernel/thread/idle.h
 * @brief Idle Threads
 */

#include <memory.h>
#include <kernel/config.h>
#include <kernel/thread/context.h>

namespace thread {

	/**
	 * @fn extern "C" void idle()
	 * @brief Idle thread function
	 */
	extern "C" void idle();

	/**
	 * @class IdleThreads
	 * @brief Container for all idle threads
	 */
	class IdleThreads {
		private:
			/**
			 * @var threads
			 * @brief Contexts of the idle threads
			 */
			Context threads[MAX_NUM_CPUS];

			/**
			 * @var stacks
			 * @brief Stack of the idle threads
			 */
			char stacks[MAX_NUM_CPUS][STACK_SIZE];

		public:
			/**
			 * @fn IdleThreads()
			 * @brief Initialize contexts of idle threads
			 */
			IdleThreads();

			/**
			 * @fn Context& get()
			 * @brief Get one idlethread context;
			 */
			Context& get();
	};

	/**
	 * @var idleThreads
	 * @brief Global idle threads
	 */
	extern IdleThreads idleThreads;

} /* namespace thread */

#endif /* ifndef _INC_KERNEL_THREAD_IDLE_H_ */
