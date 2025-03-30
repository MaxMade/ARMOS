#ifndef _INC_KERNEL_CPU_LOCAL_H_
#define _INC_KERNEL_CPU_LOCAL_H_

#include <cstdlib.h>
#include <kernel/cpu.h>
#include <kernel/config.h>

/**
 * @file kernel/cpu_local.h
 * @brief CPU local data
 */

/**
 * @class cpu_local
 * @brief CPU local data
 */
template<typename T>
class cpu_local {
	private:
		/**
		 * @var t
		 * @brief CPU local data
		 */
		T t[MAX_NUM_CPUS];

	public:
		/**
		 * @fn cpu_local()
		 * @brief Default initialization
		 */
		cpu_local() { }

		cpu_local(const cpu_local& other) = delete;

		cpu_local(cpu_local&& other) = delete;

		/**
		 * @fn cpu_local(const T& other)
		 * @brief Initialize with defaut value
		 */
		cpu_local(const T& other) {
			for (size_t i = 0; i < MAX_NUM_CPUS; i++)
				t = other;
		}

		/**
		 * @fn T& get()
		 * @brief Get CPU local data
		 */
		T& get() {
			return t[CPU::getProcessorID()];
		}

		/**
		 * @fn const T& get() const
		 * @brief Get CPU local data
		 */
		const T& get() const {
			return t[CPU::getProcessorID()];
		}
};


#endif /* ifndef _INC_KERNEL_CPU_LOCAL_H_ */
