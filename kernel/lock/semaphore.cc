#include <kernel/error.h>
#include <kernel/lock/semaphore.h>
#include <kernel/thread/scheduler.h>

using namespace lock;

Semaphore::Semaphore(int cnt) {
	val = cnt;
}

int Semaphore::p() {
	lock.lock();

	/* Must block thread */
	if (val == 0) {
		/* Get current thread */
		auto activeThread = thread::scheduler.getActive();
		assert(static_cast<bool>(activeThread));

		/* Insert into pending queue */
		if (auto ret = queue.push_back(activeThread); isError(ret)) {
			lock.unlock();
			return ret;
		}

		/* Mark thread as blocked */
		activeThread->setState(thread::State::BLOCKED);

		/* Initiate context switch and implicitly unlock */
		thread::scheduler.schedule(lock);
		return 0;
	}

	/* Update value otherwise */
	val--;
	lock.unlock();
	return 0;
}

int Semaphore::v() {
	lock.lock();

	/* Move thread from queue to scheduler */
	if (!queue.empty()) {
		/* Get first entry in queue */
		auto thread = queue.front();
		queue.pop_front();
		assert(static_cast<bool>(thread));

		/* Update state */
		thread->setState(thread::State::WAITING);

		/* Enqueue thread */
		if (auto ret = thread::scheduler.enqueue(thread); isError(ret)) {
			lock.unlock();
			return ret;
		}


	/* Update counter otherwise */
	} else {
		val++;
	}

	lock.unlock();
	return 0;
}
