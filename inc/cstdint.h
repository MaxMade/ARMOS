#ifndef _INC_CSTDINT_
#define _INC_CSTDINT_

/**
 * @file cstdint.h
 * @brief Fixed width integer types
 */

/**
 * @typedef int8_t
 * @brief Signed 8 bit integer
 */
typedef char  int8_t;

/**
 * @typedef int16_t
 * @brief Signed 16 bit integer
 */
typedef short int16_t;

/**
 * @typedef int32_t
 * @brief Signed 32 bit integer
 */
typedef int   int32_t;

/**
 * @typedef int64_t
 * @brief Signed 64 bit integer
 */
typedef long  int64_t;

/**
 * @typedef uint8_t
 * @brief Unsigned 8 bit integer
 */
typedef unsigned char  uint8_t;

/**
 * @typedef uint16_t
 * @brief Unsigned 16 bit integer
 */
typedef unsigned short uint16_t;

/**
 * @typedef uint32_t
 * @brief Unsigned 32 bit integer
 */
typedef unsigned int   uint32_t;

/**
 * @typedef uint64_t
 * @brief Unsigned 64 bit integer
 */
typedef unsigned long  uint64_t;

/**
 * @typedef intptr_t
 * @brief Signed pointer-sized integer
 */
typedef long intptr_t;

/**
 * @typedef uintptr_t
 * @brief Unsigned pointer-sized integer
 */
typedef unsigned long uintptr_t;

/**
 * @typedef intmax_t
 * @brief Max-sized signed integer
 */
typedef long long intmax_t;

/**
 * @typedef uintmax_t
 * @brief Max-sized unsigned integer
 */
typedef unsigned long long uintmax_t;

/**
 * @def INT8_MIN
 * @brief Minimum value of int8_t
 */
#define INT8_MIN -256

/**
 * @def INT8_MAX
 * @brief Maximum value of uint8_t
 */
#define INT8_MAX  255

/**
 * @def INT16_MIN
 * @brief Minimum value of int16_t
 */
#define INT16_MIN -65536

/**
 * @def INT16_MAX
 * @brief Maximum value of int16_t
 */
#define INT16_MAX  65535

/**
 * @def INT32_MIN
 * @brief Minimum value of int32_t
 */
#define INT32_MIN -4294967296

/**
 * @def INT32_MAX
 * @brief Maximum value of int32_t
 */
#define INT32_MAX  4294967295

/**
 * @def INT64_MIN
 * @brief Minimum value of int64_t
 */
#define INT64_MIN -18446744073709551616L

/**
 * @def INT64_MAX
 * @brief Maximum value of int64_t
 */
#define INT64_MAX  18446744073709551615L

/**
 * @def UINT8_MAX
 * @brief Maximum value of uint8_t
 */
#define UINT8_MAX  256

/**
 * @def UINT16_MAX
 * @brief Maximum value of uint16_t
 */
#define UINT16_MAX 65536

/**
 * @def UINT32_MAX
 * @brief Maximum value of uint32_t
 */
#define UINT32_MAX 4294967296

/**
 * @def UINT64_MAX
 * @brief Maximum value of uint64_t
 */
#define UINT64_MAX 18446744073709551616L

/**
 * @def INTMAX_MIN
 * @brief Minimum value of intmax_t
 */
#define INTMAX_MIN -18446744073709551616L

/**
 * @def INTMAX_MAX
 * @brief Maximum value of intmax_t
 */
#define INTMAX_MAX  18446744073709551615L

/**
 * @def UINTMAX_MAX
 * @brief Maximum value of uintmax_t
 */
#define UINTMAX_MAX 18446744073709551616L

/**
 * @def INTPTR_MIN
 * @brief Minimum value of intptr_t
 */
#define INTPTR_MIN -18446744073709551616L

/**
 * @def INTMAX_MAX
 * @brief Maximum value of intmax_t
 */
#define INTPTR_MAX  18446744073709551615L

/**
 * @def UINTPTR_MAX
 * @brief Maximum value of uintptr_t
 */
#define UINTPTR_MAX 18446744073709551616L

static_assert(sizeof(int8_t) == 1, "Size of int8_t must be 1 byte");
static_assert(sizeof(int16_t) == 2, "Size of int16_t must be 2 byte");
static_assert(sizeof(int32_t) == 4, "Size of int32_t must be 4 byte");
static_assert(sizeof(int64_t) == 8, "Size of int64_t must be 8 byte");

static_assert(sizeof(uint8_t) == 1, "Size of uint8_t must be 1 byte");
static_assert(sizeof(uint16_t) == 2, "Size of uint16_t must be 2 byte");
static_assert(sizeof(uint32_t) == 4, "Size of uint32_t must be 4 byte");
static_assert(sizeof(uint64_t) == 8, "Size of uint64_t must be 8 byte");

static_assert(sizeof(intptr_t) == sizeof(void *), "Size of intprt_t must match size of pointer");
static_assert(sizeof(uintptr_t) == sizeof(void *), "Size of uintprt_t must match size of pointer");

#endif /* ifndef _INC_CSTDINT_ */
