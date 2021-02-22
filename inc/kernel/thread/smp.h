#ifndef _INC_KERNEL_THREAD_SMP_H_
#define _INC_KERNEL_THREAD_SMP_H_

#include <cstdlib.h>

/**
 * @file kernel/thread/smp.h
 * @brief SMP Support for ARMOS
 * @todo Use cpu-release-addr from device tree
 */

namespace thread {
	/**
	 * @class SMP
	 * @brief Wrapper for SMP startup
	 */
	class SMP {
		private:
			/**
			 * @var apps
			 * @brief Counter of started apps
			 * @todo Should this counter be atomic?
			 */
			size_t apps;

		public:
			/**
			 * @fn SMP
			 * @brief Constructor
			 */
			SMP();

			/**
			 * @fn int start()
			 * @brief Start all applications processors
			 * @todo Ensure first CPU of cpus is boot CPU
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (errno)
			 */
			int start();

			/**
			 * @fn void registerCPU()
			 * @brief Register current CPU as application CPU
			 */
			void registerCPU();
	};
} /* namespace thread */

#endif /* ifndef _INC_KERNEL_THREAD_SMP_H_ */
