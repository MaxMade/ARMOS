#ifndef _INC_KERNEL_THREAD_CONTEXT_H_
#define _INC_KERNEL_THREAD_CONTEXT_H_

#include <cstdint.h>
#include <cstdlib.h>
#include <kernel/irq/exception_handler.h>

/**
 * @file kernel/thread/context.h
 * @brief Thread context
 */
namespace thread {

	/**
	 * @enum State
	 * @brief Execution states of a thread
	 */
	enum class State {
		CREATED,    /**< Created thread */
		RUNNING,    /**< Running thread */
		TERMINATED, /**< Terminated thread */
		WAITING,    /**< Waiting thread */
	};

	/**
	 * @struct SavedContext
	 * @brief Saved context (for context switching)
	 */
	struct SavedContext {
		uint64_t x19;
		uint64_t x20;
		uint64_t x21;
		uint64_t x22;
		uint64_t x23;
		uint64_t x24;
		uint64_t x25;
		uint64_t x26;
		uint64_t x27;
		uint64_t x28;
		uint64_t x29;
		uint64_t x30;
		uint64_t sp;
	} __attribute__((packed));

	/**
	 * @class Context
	 * @brief Thread context
	 */
	class Context {
		private:
			/**
			 * @var id
			 * @brief Thread ID
			 */
			size_t id;

			/**
			 * @var kernelStack
			 * @brief Start address of kernel stack
			 */
			void* kernelStack;

			/**
			 * @var userStack
			 * @brief Start address of user stack
			 */
			void* userStack;

			/**
			 * @var state
			 * @brief Execution state
			 */
			State state;

			/**
			 * @var savedContext
			 * @brief Saved context
			 */
			struct SavedContext savedContext;

			/**
			 * @var exceptionContext
			 * @brief Context of the exception
			 */
			irq::ExceptionContext* exceptionContext;

		public:
			/**
			 * @fn Context(size_t id, void* kernelStack, void* userStack, bool kernel, void* retAddr)
			 * @brief Constructor
			 */
			Context(size_t id, void* kernelStack, void* userStack, bool kernel, void* retAddr);

			Context(const Context& other) = delete;

			Context(Context&& other) = delete;

			/**
			 * @fn ~Context()
			 * @brief Destructor
			 */
			~Context();

			/**
			 * @fn size_t getID() const
			 * @brief Get thread ID
			 */
			size_t getID() const;

			/**
			 * @fn void setState(State state)
			 * @brief Set execution state
			 */
			void setState(State state);

			/**
			 * @fn State getState() const
			 * @brief Set thread ID
			 */
			State getState() const;

			/**
			 * @fn void setExceptionContext(irq::ExceptionContext* exceptionContext)
			 * @brief Set entry exception context
			 */
			void setExceptionContext(irq::ExceptionContext* exceptionContext);

			/**
			 * @fn irq::ExceptionContext* getExceptionContext()
			 * @brief Get entry exception context
			 */
			irq::ExceptionContext* getExceptionContext() const;

			/**
			 * @fn static void switching(Context* old, Context* next)
			 * @brief Perform context switch
			 */
			static void switching(Context* old, Context* next);
	};

} /* namespace thread */

#endif /* ifndef _INC_KERNEL_THREAD_CONTEXT_H_ */
