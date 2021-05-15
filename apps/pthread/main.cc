#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void* fn(void* arg) {
	auto id = pthread_self();
	printf("Thread: %d: %s\n\r", id, reinterpret_cast<const char*>(arg));
	while (1);
	return nullptr;
}


int main(void) {

	pthread_t id = 0;
	if (pthread_create(&id, nullptr, fn, (void*) "Hello World!") != 0) {
		perror("pthead_create");
		return 1;
	}

	printf("Thread: %d: Expected message from %d\n\r", pthread_self(), id);
	while(1);

	return 0;
}
