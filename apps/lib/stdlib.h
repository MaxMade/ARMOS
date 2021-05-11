#ifndef _INC_APPS_LIB_STDLIB_H_
#define _INC_APPS_LIB_STDLIB_H_

#include <stddef.h>

/**
 * @file apps/lib/stdlib.h
 * @brief Allocate and free dynamic memory
 */

/**
 * @fn void *malloc(size_t size)
 * @brief Allocate memory
 * @param size Number of allocated bytes
 * @return
 *	- Pointer to memory - Success
 *	-              NULL - Failure
 */
void *malloc(size_t size);

/**
 * @fn void *free(size_t size)
 * @brief size Size of allocated memory
 */
void free(void *ptr);

/**
 * @fn void *calloc(size_t nmemb, size_t size)
 * @brief Allocated memory filled with zeros
 * @param nmemb Number of elementes
 * @param size  Size of each element
 * @return
 *
 *	- Pointer to memory - Success
 *	-              NULL - Failure
 */
void *calloc(size_t nmemb, size_t size);

#endif /* ifndef _INC_APPS_LIB_STDLIB_H_ */
