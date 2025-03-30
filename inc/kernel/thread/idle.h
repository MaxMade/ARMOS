#ifndef _INC_KERNEL_THREAD_IDLE_H_
#define _INC_KERNEL_THREAD_IDLE_H_

/**
 * @file kernel/thread/idle.h
 * @brief Idle Threads
 */

#include <kernel/cpu_local.h>
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
			cpu_local<Context> threads;

		public:
			/**
			 * @fn int init()
			 * @brief Initialize contexts of idle threads
			 * @warning This function must be called on all processors
			 */
			int init();

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
