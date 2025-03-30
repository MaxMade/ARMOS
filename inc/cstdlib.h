#ifndef _INC_CSTDLIB_H_
#define _INC_CSTDLIB_H_

#include <cstddef.h>

/**
 * @file cstdlib.h
 * @brief Standard Library Definitions
 */

namespace lib {

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

	/**
	 * @fn void *realloc(void *ptr, size_t size)
	 * @brief Change size of allocation
	 * @param ptr  Pointer to memory
	 * @param size New size
	 * @return
	 *
	 *	- Pointer to memory - Success
	 *	-              NULL - Failure
	 */
	void *realloc(void *ptr, size_t size);

} /* namespace lib */

#endif /* ifndef _INC_CSTDLIB_H_ */
