#ifndef _INC_DRIVER_CPU_H_
#define _INC_DRIVER_CPU_H_

#include <cstddef.h>

/**
 * @file driver/cpu.h
 * @brief Used CPU
 * @warning Heterogenous/NUMA system are currently not supported
 */
namespace driver {

	/**
	 * @class CPU
	 * @brief Used CPU
	 */
	class CPU {
		private:
			/**
			 * @var numCores
			 * @brief Number of cores
			 */
			size_t numCores = 0;

			/**
			 * @var nameLen
			 * @brief (Arbitrary) Lenght of CPU name
			 */
			static const size_t nameLen = 80;

			/**
			 * @var name
			 * @brief String representation of CPU name
			 */
			char name[nameLen] = {0};

		public:
			/**
			 * @fn void init(const char* name, size_t cores)
			 * @brief Initialize CPU
			 */
			void init(const char* name, size_t cores);

			/**
			 * @fn const char* getName() const
			 * @brief Get CPU name
			 */
			const char* getName() const;

			/**
			 * @fn size_t getCoreCount() const
			 * @brief Get number of supported CPUs
			 */
			size_t getCoreCount() const;
	};

	extern CPU cpu;
};
#endif /* ifndef _INC_DRIVER_CPU_H_ */
