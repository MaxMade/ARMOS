#include "kernel/utility.h"
#include <cstdlib.h>
#include <cstddef.h>
#include <cstring.h>
#include <kernel/math.h>
#include <kernel/lock/spinlock.h>

/* DEFINITION -------------------------------------------------------------- */

#define MIN_ALLOC_SIZE_LOG2  4
#define MAX_ALLOC_SIZE_LOG2 27

/* Minimal allocation size (16 Bytes) */
#define MIN_ALLOC_SIZE (1 << MIN_ALLOC_SIZE_LOG2)

/* Maximum allocation size (128 MBte) */
#define MAX_ALLOC_SIZE (1 << MAX_ALLOC_SIZE_LOG2)

/* Length of free list */
#define FREE_LISTS_LEN (MAX_ALLOC_SIZE_LOG2 - MIN_ALLOC_SIZE_LOG2 + 1)

/* container_of macro */
#define container_of(ptr, type, member)                  \
	((type *) ((char *) (ptr) - offsetof(type, member)))

/* STRUCT DEFINITION ------------------------------------------------------- */

/* Embedded struct for used memory region */
struct buddy_node_used {
	size_t size;
	bool used;
	char *mem[0];
} __attribute__((packed));

/* Embbeded struct for free memory region */
struct buddy_node_free {
	size_t size;
	bool used;
	struct buddy_node_free *prev;
	struct buddy_node_free *next;
	char *mem[0];
} __attribute__((packed));

/* List of free memory including metadata about list */
struct buddy_free_list {
	struct buddy_node_free *head;
	size_t num_buddies;
};

/* VARIABLES --------------------------------------------------------------- */

/* Array of free list for MIN_ALLOC_SIZE - MAX_ALLOC_SIZE */
static struct buddy_free_list free_lists[FREE_LISTS_LEN];

/* Memory of allocator */
static char __mem[MAX_ALLOC_SIZE];
static size_t __size = MAX_ALLOC_SIZE;

/* Synchronization lock */
static lock::spinlock allocLock;

/* HELPER ------------------------------------------------------------------ */

/* Convert size to index in buddy_free_lists */
static size_t __size_to_index(size_t size) {
	return math::ld(size) - MIN_ALLOC_SIZE_LOG2;
}

/* Find associated buddy */
static void *__find_buddy(void *ptr) {
	struct buddy_node_used *buddy = (struct buddy_node_used *) ptr;
	uintptr_t offset = ((uintptr_t) ptr) - ((uintptr_t) __mem);
	uintptr_t offset_buddy = offset ^ buddy->size;

	return (void *) ((uintptr_t) __mem + offset_buddy);
}

/* Remove buddy from free_lists */
static void __remove_buddy(struct buddy_node_free *buddy) {
	struct buddy_node_free *prev = buddy->prev;
	struct buddy_node_free *next = buddy->next;
	ssize_t idx = __size_to_index(buddy->size);

	/* Update prev and head */
	if (prev != nullptr) {
		prev->next = next;
	} else {
		free_lists[idx].head = next;
	}

	/* Update next */
	if (next != nullptr) {
		next->prev = prev;
	}

	/* Prepare buddy */
	buddy->next = nullptr;
	buddy->prev = nullptr;

	/* Update num_buddies */
	free_lists[idx].num_buddies--;
}

/* Insert buddy into free_lists */
static void __insert_buddy(struct buddy_node_free *buddy) {
	ssize_t idx = __size_to_index(buddy->size);

	/* Update buddy */
	struct buddy_node_free *head = free_lists[idx].head;
	buddy->next = head;
	buddy->prev = nullptr;

	/* Update old head */
	if (head != nullptr) {
		head->prev = buddy;
	}

	/* Update head */
	free_lists[idx].head = buddy;

	/* Update num_buddies */
	free_lists[idx].num_buddies++;
}

/* Remove buddy from free_lists and split it */
static void __split_buddy(struct buddy_node_free *buddy) {
	/* Remove buddy from free_list */
	__remove_buddy(buddy);

	/* Calculate left and right buddy */
	size_t new_size = buddy->size / 2;
	struct buddy_node_free *left = buddy;
	struct buddy_node_free *right = (struct buddy_node_free *) (((uintptr_t) buddy) + new_size);

	/* Prepare left buddy */
	left->size = new_size;
	left->used = false;
	left->next = nullptr;
	left->prev = nullptr;

	/* Prepare right buddy */
	right->size = new_size;
	right->used = false;
	right->next = nullptr;
	right->prev = nullptr;

	/* Return left/right */
	__insert_buddy(right);
	__insert_buddy(left);
}

/* Merge buddy (which isn't part of ready list) */
static struct buddy_node_free *__merge_buddy(struct buddy_node_free *buddy) {
	/* Can't merge more */
	if (buddy->size == __size)
		return nullptr;

	/* Find matching (free) buddy */
	struct buddy_node_free *match = (struct buddy_node_free*) __find_buddy(buddy);
	if (match->used == true)
		return nullptr;
	if (buddy->size != match->size)
		return nullptr;
	__remove_buddy(match);

	/* Merge */
	size_t new_size = buddy->size * 2;
	struct buddy_node_free *ret = (uintptr_t) buddy < (uintptr_t) match ? buddy : match;
	ret->size = new_size;
	ret->prev = nullptr;
	ret->next = nullptr;
	return ret;
}

__attribute__((constructor))
static void buddy_init() {
	size_t idx = MAX_ALLOC_SIZE_LOG2 - MIN_ALLOC_SIZE_LOG2;

	/* Prepare freelist */
	memset(&free_lists, 0, sizeof(struct buddy_free_list) * FREE_LISTS_LEN);

	/* Prepare root */
	struct buddy_node_free *root = (struct buddy_node_free *) __mem;
	root->size = __size;
	root->used = false;
	root->next = nullptr;
	root->prev = nullptr;

	/* Insert into free_lists */
	free_lists[idx].head = root;
	free_lists[idx].num_buddies = 1;
}

static void *buddy_malloc(size_t size) {
	size_t actual_size = size + sizeof(struct buddy_node_used);
	if (!math::isPowerOfTwo(actual_size))
		actual_size = 1 << (util::fls(actual_size) + 1);

	actual_size = actual_size < MIN_ALLOC_SIZE ? MIN_ALLOC_SIZE : actual_size;
	size_t idx = __size_to_index(actual_size);

	/* Find fitting buddy */
	ssize_t found_index = -1;
	size_t num_splits = 0;
	for (size_t i = idx; i < FREE_LISTS_LEN; i++) {
		if (free_lists[i].num_buddies > 0) {
			found_index = i; break;
		}

		num_splits++;
	}

	/* Check if suitable memory was found */
	if (found_index == -1)
		return nullptr;


	/* Split */
	for (size_t i = 0; i < num_splits; i++) {
		__split_buddy(free_lists[found_index - i].head);
	}

	/* Retrieve buddy */
	struct buddy_node_free *buddy = free_lists[found_index - num_splits].head;
	__remove_buddy(buddy);

	struct buddy_node_used *ret = (struct buddy_node_used *) buddy;
	ret->size = actual_size;
	ret->used = true;

	return ret->mem;
}

static void buddy_free(void *ptr) {
	if (ptr == nullptr)
		return;


	/* Get buddy */
	struct buddy_node_used *used_buddy = container_of(ptr, struct buddy_node_used, mem);
	size_t size = used_buddy->size;

	/* Prepare free buddy */
	struct buddy_node_free *free_buddy = (struct buddy_node_free *) used_buddy;
	free_buddy->size = size;
	free_buddy->used = false;
	free_buddy->next = nullptr;
	free_buddy->prev = nullptr;

	/* Merge */
	struct buddy_node_free *tmp = nullptr;
	while((tmp = __merge_buddy(free_buddy)) != nullptr)
		free_buddy = tmp;

	/* Insert */
	__insert_buddy(free_buddy);
}

static void *buddy_calloc(size_t nmemb, size_t size) {
	if (nmemb == 0 || size == 0) {
		return nullptr;
	}

	void *ret = buddy_malloc(nmemb * size);
	if (ret != nullptr)
		memset(ret, 0, nmemb * size);

	return ret;
}

static void *buddy_realloc(void *ptr, size_t size) {
	if (ptr == nullptr) {
		void *ret = buddy_malloc(size);
		return ret;
	}

	if (size == 0) {
		buddy_free(ptr);
		return nullptr;
	}

	/* Check if ptr is valid */

	struct buddy_node_used *tmp = container_of(ptr, struct buddy_node_used, mem);


	/* Check if reallocation is necessary */
	if (tmp->size >= size + sizeof(struct buddy_node_used))
		return ptr;

	void *ret = buddy_malloc(size);
	if (!ret) {
		memcpy(ret, ptr, tmp->size - sizeof(struct buddy_node_used));
		buddy_free(ptr);
	}
	return ret;
}

void *lib::malloc(size_t size) {
	allocLock.lock();
	void *ret = buddy_malloc(size);
	allocLock.unlock();
	return ret;
}

void lib::free(void *ptr) {
	allocLock.lock();
	buddy_free(ptr);
	allocLock.unlock();
}

void *lib::calloc(size_t nmemb, size_t size) {
	allocLock.lock();
	void *ret = buddy_calloc(nmemb, size);
	allocLock.unlock();
	return ret;
}

void *lib::realloc(void *ptr, size_t size) {
	allocLock.lock();
	void *ret = buddy_realloc(ptr, size);
	allocLock.unlock();
	return ret;
}
