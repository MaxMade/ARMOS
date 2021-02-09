#ifndef _INC_DRIVER_SYSTEM_TIMER_H_
#define _INC_DRIVER_SYSTEM_TIMER_H_

#include <cstdint.h>
#include <kernel/utility.h>
#include <driver/config.h>
#include <driver/generic_timer.h>

/**
 * @file driver/system_timer.h
 * @brief Driver for BCM2835 System Timer
 */

namespace driver {

	class system_timer : public generic_timer {
		private:
			/**
			 * @var base
			 * @brief Base address of configuration space
			 */
			void* base;

			/**
			 * @var intConfig
			 * @brief Interrupt configuration
			 */
			lib::pair<void*, size_t> intConfig;

			/**
			 * @enum regOffset
			 * @brief Offsets for registers
			 */
			typedef enum : int16_t {
				CS	= 0x00, /**< System Timer Control/Status */
				CLO = 0x04, /**< System Timer Counter Lower */
				CHI = 0x08, /**< System Timer Counter Higher */
				C0  = 0x0C, /**< System Timer Compare 0 */
				C1  = 0x10, /**< System Timer Compare 1 */
				C2  = 0x14, /**< System Timer Compare 2 */
				C3  = 0x18, /**< System Timer Compare 3 */
			} regOffset;

			/**
			 * @fn void writeRegister(uint32_t value)
			 * @brief Internal write register
			 */
			template<regOffset off>
			void writeRegister(uint32_t value) {
				uint32_t* reg = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(base) + off);
				util::mmioWrite(reg, value);
			}

			/**
			 * @fn void readRegister(uint32_t value)
			 * @brief Internal read register
			 */
			template<regOffset off>
			uint32_t readRegister() {
				uint32_t* reg = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(base) + off);
				return util::mmioRead(reg);
			}

			/**
			 * var intv
			 * @brief Configured interval
			 */
			uint32_t intv;

			/**
			 * @var ts
			 * @brief Current time stamp
			 */
			uint32_t ts;

			/**
			 * @var ticks;
			 * @brief Number of ticks
			 */
			size_t ticks;

			/**
			 * @var MAX_CALLBACKS
			 * @brief Number of registable callbacks
			 */
			static const size_t MAX_CALLBACKS = 10;

			/**
			 * @var callbacks
			 * @brief Pair of number of (needed) ticks and the actual callback
			 */
			lib::pair<size_t, lib::function<int(void)>> callbacks[MAX_CALLBACKS];

			/**
			 * @var idxCallback
			 * @brief Current index in callbacks
			 */
			size_t idxCallback;

		public:
			/**
			 * @fn system_timer
			 * @brief Constuctor of driver with sets name
			 */
			system_timer();

			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize Timer
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int windup(size_t ms)
			 * @brief Windup and activate timer
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int windup(size_t ms);

			/**
			 * @fn size_t interval() const
			 * @brief Get current configured interval (in ms)
			 */
			size_t interval() const;

			/**
			 * @fn int registerFunction(size_t ms, lib::function<int(void)> callback)
			 * @brief Register callback which is executed in a regular interval
			 * @warning ms must be multiple of interval
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerFunction(size_t ms, lib::function<int(void)> callback);

			/**
			 * @fn lib::pair<void*, size_t> getConfigSpace() const
			 * @brief Get used address range
			 */
			lib::pair<void*, size_t> getConfigSpace() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_SYSTEM_TIMER_H_ */
