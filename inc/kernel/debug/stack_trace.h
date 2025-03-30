#ifndef _INC_KERNEL_DEBUG_H_
#define _INC_KERNEL_DEBUG_H_

#include <cstddef.h>
#include <ostream.h>
#include <kernel/symbols.h>
#include <kernel/irq/exception_handler.h>
#include <hw/register/general_purpose_reg.h>

/**
 * @file kernel/debug/stack_trace.h
 * @brief Kernel-space stack trace
 */

/* Outermost frames */
extern int kernelMain(void *fdt);
extern int kernelMainApp();
extern "C" void current_el_sp_el0_sync(void* saved_state);
extern "C" void current_el_sp_el0_irq(void* saved_state);
extern "C" void current_el_sp_el0_fiq(void* saved_state);
extern "C" void current_el_sp_el0_serror(void* saved_state);
extern "C" void current_el_sp_elx_sync(void* saved_state);
extern "C" void current_el_sp_elx_irq(void* saved_state);
extern "C" void current_el_sp_elx_fiq(void* saved_state);
extern "C" void current_el_sp_elx_serror(void* saved_state);
extern "C" void lower_el_aarch64_sync(void* saved_state);
extern "C" void lower_el_aarch64_irq(void* saved_state);
extern "C" void lower_el_aarch64_fiq(void* saved_state);
extern "C" void lower_el_aarch64_serror(void* saved_state);
extern "C" void lower_el_aarch32_sync(void* saved_state);
extern "C" void lower_el_aarch32_irq(void* saved_state);
extern "C" void lower_el_aarch32_fiq(void* saved_state);
extern "C" void lower_el_aarch32_serror(void* saved_state);

namespace debug {

	/**
	 * @struct func_prolog
	 * @brief Register pushed onto stack by function prolog
	 */
	struct func_prolog {
		void* fp;
		void* lr;
	} __attribute((packed));


	/**
	 * @fn void stack_trace(size_t maxFrames, void* fp = nullptr)
	 * @brief Generate stack trace
	 * @tparam ostream Used ostream (lib::ostream or lib::panic)
	 * @tparam flush   Flag to flush stream
	 * @param maxFrames Max. number of observed frames
	 * @param fp        Use fp instead of current frame pointer (if non nullptr)
	 */
	template<typename ostream = lib::panic, bool flush = false>
	void stack_trace(size_t maxFrames, void* fp = nullptr) {
		/* Get current frame pointer */
		if (fp == nullptr)
			x29(fp);

		/* Get output stream */
		ostream stream;
		stream.setf(lib::ostream::showbase);
		stream.setf(lib::ostream::hex, lib::ostream::basefield);

		func_prolog* frame = reinterpret_cast<func_prolog*>(fp);
		for (size_t i = 0; i < maxFrames; i++) {
			void* addr = frame->lr;
			/* Print frame number */
			stream.width(5);
			stream << i;
			stream.width(0);
			stream << ": ";

			/* Check if symbol is found */
			auto sym = symbols.lookup(addr);
			if (sym.first == nullptr) {
				stream << "Abort: Found non-existing function (" << addr << ")\n\r";
				break;
			}

			/* Print Symbol */
			stream << sym.first << "+" << sym.second;

			/* Print newline */
			stream << "\n\r";

			/* Check if outermost frame is reached */
			uintptr_t funcAddress = reinterpret_cast<uintptr_t>(addr) - sym.second;
			if (funcAddress == reinterpret_cast<uintptr_t>(kernelMain) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_serror)) {

				break;
			}

			/* Exaime next frame */
			frame = reinterpret_cast<func_prolog*>(frame->fp);
		}

		if constexpr(flush)
			stream.flush();
	}

	/**
	 * @fn void stack_trace(size_t maxFrames, irq::ExceptionContext* context)
	 * @brief Generate stack trace from exception context
	 * @tparam ostream Used ostream (lib::ostream or lib::panic)
	 * @tparam flush   Flag to flush stream
	 * @param maxFrames Max. number of observed frames
	 * @param context   Pointer to exception context
	 */
	template<typename ostream = lib::panic, bool flush = false>
	void stack_trace(size_t maxFrames, irq::ExceptionContext* context) {
		/* Get current frame pointer */
		auto fp = context->x29;

		/* Get output stream */
		ostream stream;
		stream.setf(lib::ostream::showbase);
		stream.setf(lib::ostream::hex, lib::ostream::basefield);

		/* Print outer-most frame */
		stream.width(5);
		stream << 0;
		stream.width(0);
		stream << ": ";

		/* Print outermost frame */
		auto addr = reinterpret_cast<void*>(context->elr_el1);
		auto sym = symbols.lookup(addr);
		if (sym.first == nullptr) {
			stream << "Abort: Found non-existing function (" << addr << ")\n\r";
			if constexpr(flush)
				stream.flush();
			return;
		}

		/* Print Symbol */
		stream << sym.first << "+" << sym.second;

		/* Print newline */
		stream << "\n\r";

		func_prolog* frame = reinterpret_cast<func_prolog*>(fp);
		for (size_t i = 1; i < maxFrames; i++) {
			addr = frame->lr;
			/* Print frame number */
			stream.width(5);
			stream << i;
			stream.width(0);
			stream << ": ";

			/* Check if symbol is found */
			sym = symbols.lookup(addr);
			if (sym.first == nullptr) {
				stream << "Abort: Found non-existing function (" << addr << ")\n\r";
				break;
			}

			/* Print Symbol */
			stream << sym.first << "+" << sym.second;

			/* Print newline */
			stream << "\n\r";

			/* Check if outermost frame is reached */
			uintptr_t funcAddress = reinterpret_cast<uintptr_t>(addr) - sym.second;
			if (funcAddress == reinterpret_cast<uintptr_t>(kernelMain) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_el0_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(current_el_sp_elx_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch64_serror) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_sync) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_irq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_fiq) ||
				funcAddress == reinterpret_cast<uintptr_t>(lower_el_aarch32_serror)) {

				break;
			}

			/* Exaime next frame */
			frame = reinterpret_cast<func_prolog*>(frame->fp);
		}

		if constexpr(flush)
			stream.flush();
	}

} /* namespace debug */

#endif /* ifndef _INC_KERNEL_DEBUG_H_ */
