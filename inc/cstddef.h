#ifndef _INC_CSTDDEF_H_
#define _INC_CSTDDEF_H_

/**
 * @file cstddef.h
 * @brief Standard type definitions
 */

/**
 * @def NULL
 * @brief Null pointer constant
 */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

/**
 * @typedef size_t
 * @brief Unsigned integer type of the result of \c sizeof operator
 */
#ifndef size_t
	typedef unsigned long size_t;
#endif

/**
 * @typedef ssize_t
 * @brief Signed integer type
 */
#ifndef ssize_t
	typedef long ssize_t;
#endif

/**
 * @typedef ptrdiff_t
 * @brief Result of substracting two pointers
 */
#ifndef ptrdiff_t
	typedef long ptrdiff_t;
#endif

/**
 * @def offsetof(type, member)
 * @brief Offset of struct member form the beginning of its structure
 * @param type   Structure type
 * @param member structure member
 */
#ifndef offsetof
	#define offsetof(type, member) ((size_t) &((type *) 0)->member)
#endif

#endif /* _INC_CSTDDEF_H_ */
