#ifndef _INC_DRIVER_CPU_H_
#define _INC_DRIVER_CPU_H_

#include <cstddef.h>
#include <kernel/config.h>

/**
 * @file driver/cpu.h
 * @brief Used CPU
 */
namespace driver {

	/**
	 * @class CPU
	 * @brief Used CPU
	 */
	class CPU {
		private:
			/**
			 * @var nameLen
			 * @brief Max name length
			 */
			static const size_t nameLen = 80;

			/**
			 * @var name
			 * @brief Symbolic name of CPU
			 */
			char name[nameLen];

			/**
			 * @var spintable
			 * @brief Pointer to spintable entry
			 */
			void* spintable;

		public:
			CPU() = default;

			/**
			 * @fn CPU(const char* name, void* spintable = nullptr)
			 * @brief
			 */
			CPU(const char* name, void* spintable = nullptr);

			/**
			 * @fn const char* getName() const
			 * @brief
			 */
			const char* getName() const;

			/**
			 * @fn void* getSpintable() const
			 * @brief Get spintable entry
			 */
			void* getSpintable() const;
	};

	/**
	 * @class CPUs
	 * @brief List of all used CPUs
	 */
	class CPUs {
		private:
			/**
			 * @var idx
			 * @brief Currently used index in cpus
			 */
			size_t idx;

			/**
			 * @var cpus
			 * @brief List of CPUs
			 */
			CPU cpus[MAX_NUM_CPUS];

		public:
			/**
			 * @typedef iterator
			 * @brief Used iterator
			 */
			using iterator = const CPU*;

			/**
			 * @fn CPUs
			 * @brief Constructor
			 */
			CPUs();

			/**
			 * @fn int registerCPU(CPU cpu)
			 * @brief Register new CPU
			 */
			int registerCPU(CPU cpu);

			/**
			 * @fn size_t numCPUs() const
			 * @brief Get total number of CPUs
			 */
			size_t numCPUs() const;

			/**
			 * @fn iterator begin() const
			 * @brief Iterator for CPUs
			 */
			iterator begin() const;

			/**
			 * @fn iterator end() const
			 * @brief Iterator for CPUs
			 */
			iterator end() const;
	};

	/**
	 * @var cpus
	 * @brief Global CPU list
	 */
	extern CPUs cpus;
};
#endif /* ifndef _INC_DRIVER_CPU_H_ */
