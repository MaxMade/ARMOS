#include <cerrno.h>
#include <climits.h>
#include <cstdlib.h>
#include <kernel/error.h>
#include <kernel/mm/paging.h>
#include <kernel/thread/scheduler.h>

using namespace thread;

Scheduler::Scheduler() {}

int Scheduler::create(void* (*start_routine)(void*), void* arg) {
	/* Allocate user stack */
	auto userStack = reinterpret_cast<char*>(lib::memalign(PAGESIZE, STACK_SIZE));
	if (userStack == nullptr)
		return -ENOMEM;

	/* Allocate kernel stack */
	auto kernelStack = new char[STACK_SIZE];
	if (kernelStack == nullptr) {
		lib::free(userStack);
		return -ENOMEM;
	}

	/* Prepare new thread */
	lock.lock();
	
	/* Find unused ID */
	size_t id = 0;
	if (auto it = threads.begin(); it != threads.end()) {
		id = it->first;
	}
	id++;

	if (id >= MAX_TID) {
		lib::free(kernelStack);
		lock.unlock();
		return -ENOSPC;
	}


	/* Allocate new Context */
	auto context = new Context(id, kernelStack, userStack, false, (void*) start_routine, arg);
	if (context == nullptr) {
		lib::free(userStack);
		lib::free(kernelStack);
		lock.unlock();
		return -ENOMEM;
	}

	/* Create new smart ptr */
	lib::shared_ptr<Context> thread(context);
	if (!static_cast<bool>(thread)) {
		delete context;
		lock.unlock();
		return -ENOMEM;
	}

	/* Insert into ready queue */
	if (int ret = 0; (ret = queue.push_back(thread)) != 0) {
		delete context;
		lock.unlock();
		return ret;
	}

	/* Insert into thread set */
	if (int ret = 0; (ret = threads.emplace(id, thread)) != 0) {
		delete context;
		queue.pop_back();
		lock.unlock();
		return ret;
	}

	/* Update protection of user stack */
	mm::Paging paging;
	for (size_t i = 0; i < STACK_SIZE / PAGESIZE; i++) {
		auto ret = paging.protect(&userStack[i * PAGESIZE], mm::Paging::USER_MAPPING, mm::Paging::WRITABLE, mm::Paging::NORMAL_ATTR);
		assert(!isError(ret));
		CPU::invalidatePage(&userStack[i * PAGESIZE]);
	}

	lock.unlock();
	return 0;
}

lib::shared_ptr<Context> Scheduler::getActive() {
	return lib::shared_ptr<Context>(activeThreads.get());
}

lib::shared_ptr<Context> Scheduler::getThread(size_t id) {
	lock.lock();
	auto it = threads.find(id);
	if (it == threads.end()) {
		lock.unlock();
		return lib::shared_ptr<Context>();
	}

	lib::shared_ptr<Context> ret(it->second);
	lock.unlock();
	return ret;
}

void Scheduler::schedule() {
	lock.lock();

	/* Check queue */
	if (queue.size() == 0) {
		lock.unlock();
		return;
	}

	/* Remove first entry */
	auto it = queue.begin();
	lib::shared_ptr<Context> thread(*it);
	auto ret = queue.pop_front();
	assert(ret);

	/* Update state */
	thread->setState(State::RUNNING);

	/* Get current thread */
	if (static_cast<bool>(activeThreads.get())) {
		activeThreads.get()->setState(State::WAITING);
		thread.swap(activeThreads.get());
		if (auto ret = queue.push_back(thread); isError(ret))
			debug::panic::generate("Unable to enqueue thread into ready queue", ret);


		Context::switching(thread.get(), activeThreads.get().get());

	} else {
		thread.swap(activeThreads.get());
		Context tmpContext;
		Context::switching(&tmpContext, activeThreads.get().get());
	}

	lock.unlock();
}

void Scheduler::__unlock() {
	lock.unlock();
}
