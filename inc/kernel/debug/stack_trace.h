#ifndef _INC_KERNEL_DEBUG_H_
#define _INC_KERNEL_DEBUG_H_

#include <cstddef.h>
#include <ostream.h>
#include <kernel/symbols.h>
#include <kernel/debug/frame_magic.h>
#include <hw/register/general_purpose_reg.h>

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
	 * @fn void stack_trace(size_t maxFrames)
	 * @brief Generate stack trace
	 * @tparam ostream Used ostream (lib::ostream or lib::panic)
	 * @tparam flush   Flag to flush stream
	 */
	template<typename ostream = lib::panic, bool flush = false>
	void stack_trace(size_t maxFrames) {
		/* Get current stack pointer */
		void* fp = 0;
		x29(fp);

		/* Get output stream */
		ostream stream;
		stream.setf(lib::ostream::showbase);
		stream.setf(lib::ostream::hex, lib::ostream::basefield);


		func_prolog* frame = reinterpret_cast<func_prolog*>(fp);
		for (size_t i = 0; i < maxFrames && reinterpret_cast<uintptr_t>(frame->fp) != FRAME_MAGIC; i++) {
			void* addr = frame->lr;
			/* Print frame number */
			stream.width(5);
			stream << i;
			stream.width(0);
			stream << ": ";

			/* Print symbol or address */
			auto sym = symbols.lookup(addr);
			if (sym.first == nullptr) {
				stream << sym.first;
			} else {
				stream << sym.first << "+" << sym.second;
			}

			/* Print newline */
			stream << "\n\r";

			/* Exaime next frame */
			frame = reinterpret_cast<func_prolog*>(frame->fp);
		}

		if constexpr(flush)
			stream.flush();
	}

} /* namespace debug */

#endif /* ifndef _INC_KERNEL_DEBUG_H_ */
