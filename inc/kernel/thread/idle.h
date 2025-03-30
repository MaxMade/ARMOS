#ifndef _INC_KERNEL_THREAD_IDLE_H_
#define _INC_KERNEL_THREAD_IDLE_H_

/**
 * @file kernel/thread/idle.h
 * @brief Idle Thread
 */

namespace thread {

	/**
	 * @fn extern "C" void idle()
	 * @brief Idle Thread
	 */
	extern "C" void idle();

} /* namespace thread */

#endif /* ifndef _INC_KERNEL_THREAD_IDLE_H_ */
