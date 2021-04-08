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

#endif /* ifndef _APP_LIB_PTHREAD_H_ */
