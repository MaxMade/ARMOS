#ifndef _APP_LIB_PTHREAD_H_
#define _APP_LIB_PTHREAD_H_

/**
 * @typedef pthread_t
 * @brief Thread ID
 */
typedef unsigned long int pthread_t;

/**
 * @fn pthread_t pthread_self(void)
 * @brief Return ID of the calling thread
 */
pthread_t pthread_self(void);

/**
 * @typedef pthread_spinlock_t
 * @brief Pthread Spinlock
 */
typedef  bool pthread_spinlock_t;

/**
 * @typedef pthread_attr_t
 * @brief Pthread Attributes
 * @todo Implement me
 */
struct pthread_attr {};
typedef struct pthread_attr pthread_attr_t;

/**
 * @fn int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
 * @brief Initialize lock
 * @warning \c pshared will be ignored!
 * @return
 *	- 0      - Success
 *	- EAGAIN - The system has insufficient resources to initialize a new spin lock
 *	- ENOMEM - Insufficient memory to initialize the spin lock
 */
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);

/**
 * @fn int pthread_spin_destroy(pthread_spinlock_t *lock)
 * @brief Destory created spinlock
 * @return
 *	- 0 - Success
 */
int pthread_spin_destroy(pthread_spinlock_t *lock);

/**
 * @fn int pthread_spin_lock(pthread_spinlock_t *lock)
 * @brief Lock spinlock
 * @return
 *
 *	- 0        - Success
 *	- EDADLOCK - The system detected a deadlock condition
 */
int pthread_spin_lock(pthread_spinlock_t *lock);

/**
 * @fn int pthread_spin_trylock(pthread_spinlock_t *lock)
 * @brief Try to lock spinlock
 * @return
 *
 *	- 0         - Success
 *	- EDEADLOCK - The system dected a deadlock condition
 *	- EBUSY     - The spinlock is currently locked by another thread
 */
int pthread_spin_trylock(pthread_spinlock_t *lock);

/**
 * @fn int pthread_spin_trylock(pthread_spinlock_t *lock)
 * @brief Unlock spinlock
 * @return
 *
 *	- 0 - Success
 */
int pthread_spin_unlock(pthread_spinlock_t *lock);

/**
 * @fn int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
 * @brief Create new thead
 * @param thread        Reference of created thread
 * @param attr          Attributes for created thread (currently ignored!)
 * @param start_routine Executed routine
 * @param arg           Arguments passed to start_routine
 * @warning attr will be ignored!
 * @return
 *
 *	- 0      - Success
 *	- EAGAIN - The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process {PTHREAD_THREADS_MAX} would be exceeded.
 *	- EPERM  - The caller does not have appropriate privileges to set the required scheduling parameters or scheduling policy.
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

#endif /* ifndef _APP_LIB_PTHREAD_H_ */
