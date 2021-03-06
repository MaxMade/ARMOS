#ifndef _INC_DRIVER_MAILBOX_H_
#define _INC_DRIVER_MAILBOX_H_

#include "kernel/lock/spinlock.h"
#include <atomic.h>
#include <cstdint.h>
#include <cstddef.h>
#include <functional.h>
#include <kernel/utility.h>
#include <driver/config.h>
#include <driver/generic_ipi.h>

/**
 * @file driver/mailbox.h
 * @brief BCM2835 Mailbox
 */

namespace driver {

	/**
	 * @class mailbox
	 * @brief BCM2835 Mailbox
	 */
	class mailbox : public generic_ipi {
		private:
			/**
			 * @var base
			 * @brief Base address
			 */
			void* base;

			/**
			 * @var intConfig
			 * @brief Interrupt configuration
			 */
			lib::pair<void*, size_t> intConfig;

			/**
			 * @var messages
			 * @brief Buffered messages
			 */
			lib::atomic<uint32_t> messages[4];

			lib::atomic<size_t> msgCounter;

			lock::spinlock lock;

			/**
			 * @typedef regOffset
			 * @brief Memory mapped registers
			 */
			typedef enum : uint32_t {
				MAILBOX_WRITE_CORE_0 = 0x00000080, /**< Core 0 Mailbox 0 write-set (WO) */
				MAILBOX_WRITE_CORE_1 = 0x00000090, /**< Core 1 Mailbox 0 write-set (WO) */
				MAILBOX_WRITE_CORE_2 = 0x000000A0, /**< Core 2 Mailbox 0 write-set (WO) */
				MAILBOX_WRITE_CORE_3 = 0x000000B0, /**< Core 3 Mailbox 0 write-set (WO) */

				MAILBOX_READ_CORE_0  = 0x000000C0, /**< Core 0 Mailbox 0 read & write-high-to-clear */
				MAILBOX_READ_CORE_1  = 0x000000D0, /**< Core 1 Mailbox 0 read & write-high-to-clear */
				MAILBOX_READ_CORE_2  = 0x000000E0, /**< Core 2 Mailbox 0 read & write-high-to-clear */
				MAILBOX_READ_CORE_3  = 0x000000F0, /**< Core 3 Mailbox 0 read & write-high-to-clear */

				MAILBOX_IRQ_CORE_0   = 0x00000050, /**< Core0 Mailboxes Interrupt control */
				MAILBOX_IRQ_CORE_1   = 0x00000054, /**< Core1 Mailboxes Interrupt control */
				MAILBOX_IRQ_CORE_2   = 0x00000058, /**< Core2 Mailboxes Interrupt control */
				MAILBOX_IRQ_CORE_3   = 0x0000005C, /**< Core3 Mailboxes Interrupt control */
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
			 * @fn void enableIRQ(size_t core)
			 * @brief Enable IRQs
			 */
			void enableIRQ(size_t core);

			static const size_t numHandlers = 4;

			lib::pair<IPI_MSG, lib::function<int()>> handlers[numHandlers];

		public:
			/**
			 * @fn mailbox
			 * @brief Construct mailbox
			 */
			mailbox();

			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize IPI driver
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int sendIPI(size_t cpuID, IPI_MSG msg)
			 * @brief Send IPI to cpu
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int sendIPI(size_t cpuID, IPI_MSG msg);

			/**
			 * @fn int registerHandler(IPI_MSG msg, lib::function<int()> handler)
			 * @brief Register Handler for specific value
			 */
			int registerHandler(IPI_MSG msg, lib::function<int()> handler);

			/**
			 * @fn int prologue(irq::ExceptionContext* context) override
			 * @brief Exception prologue
			 * @return
			 *
			 *	-  1 - Epilogue is needed
			 *	-  0 - Epilogue isn't needed
			 *	- <0 - Error (errno)
			 */
			int prologue(irq::ExceptionContext* context) override;

			/**
			 * @fn int epilogue() override
			 * @brief Exception epilogue
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (errno)
			 */
			int epilogue() override;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_MAILBOX_H_ */
