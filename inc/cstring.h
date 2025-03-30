#ifndef _INCLUDE_CSTRING_H_
#define _INCLUDE_CSTRING_H_

/**
 * @file cstring.h
 * @brief String operations
 */

#include <cstddef.h>

/**
 * @fn void *memchr(const void *s, int c, size_t n)
 * @brief Scan for a character
 * @param s Memory area
 * @param c Searched character
 * @param n Size of s
 * @return
 *
 *	- Pointer to matching byte - Success
 *	-                     NULL - Failure
 */
void *memchr(const void *s, int c, size_t n);

/**
 * @fn int memcmp(const void *s1, const void *s2, size_t n)
 * @brief Compare first \c n byte of \c s1 and \c s2
 * @param s1 First string
 * @param s2 Second string
 * @param n  Number of compared bytes
 * @return
 *
 *	- < 0 - First n characters of s1 are less than the first n characters of s2
 *	-   0 - First n characters of s1 are equal to the first n characters of s2
 *	- > 0 - First n characters of s1 are greater than the first n characters of s2
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * @fn void *memcpy(void *dest, const void *src, size_t n)
 * @brief Copy memory area
 * @param dest Destination memory
 * @param src  Source memory
 * @param n    Number of bytes
 * @note The memory areas must not overlay
 * @return
 *
 *	- Pointer to \c dest
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @fn void *memmove(void *dest, const void *src, size_t n)
 * @brief Copy memory area
 * @param dest Destination memory
 * @param src  Source memory
 * @param n    Number of bytes
 * @return
 *
 *	- Pointer to \c dest
 */
void *memmove(void *dest, const void *src, size_t n);

/**
 * @fn void *memset(void *dest, int c, size_t n)
 * @brief Fill memory with a constant byte
 * @param dest Destination memory
 * @param c    Constant byte
 * @param n    Number of bytes
 * @return
 *
 *	- Pointer to \c dest
 */
void *memset(void *dest, int c, size_t n);

/**
 * @fn char *strcat(char *dest, const char *src)
 * @brief Concatenate two strings (appends terminating null byte)
 * @param dest Destination string
 * @param src  Source string
 * @warning \c dest must have enough space for the result
 * @return
 *
 *	- Pointer to \c dest
 */
char *strcat(char *dest, const char *src);

/**
 * @fn char *strncat(char *dest, const char *src, size_t n)
 * @brief Concatenate two strings (appends terminating null byte)
 * @param dest Destination string
 * @param src  Source string
 * @param n    Upper limit of copied bytes (from \c src)
 * @warning \c dest must have enough space for the result
 * @note If src contains n or more bytes, strncat will write n+1 bytes
 * (including terminating null byte)
 * @return
 *
 *	- Pointer to \c dest
 */
char *strncat(char *dest, const char *src, size_t n);

/**
 * @fn int strcmp(const char *s1, const char *s2)
 * @brief Compare two strings
 * @param s1 First string
 * @param s2 Second string
 * @return
 *
 *	- < 0 - \c s1 is less than \c s2
 *	-   0 - \c s1 is equal to \c s2
 *	- > 0 - \c s2 is greater than \c s2
 */
int strcmp(const char *s1, const char *s2);

/**
 * @fn int strncmp(const char *s1, const char *s2, size_t n)
 * @brief Compare two strings
 * @param s1 First string
 * @param s2 Second string
 * @param n  Upper limit of compared bytes
 * @return
 *
 *	- < 0 - \c s1 is less than \c s2
 *	-   0 - \c s1 is equal to \c s2
 *	- > 0 - \c s2 is greater than \c s2
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * @fn char *strcpy(char *dest, const char *src)
 * @brief Copy strings
 * @param dest Destination string
 * @param src  Source string
 * @warning Beware of buffer overruns
 * @return
 *
 *	- Pointer to \c dest
 */
char *strcpy(char *dest, const char *src);

/**
 * @fn char *strncpy(char *dest, const char *src, size_t n)
 * @brief Copy strings
 * @param dest Destination string
 * @param src  Source string
 * @param n    Upper limit of copied characters
 * @warning Beware of buffer overruns
 * @note If length of \c src is greater than \c n, strncpy won't append a
 * terminating null byte
 * @return
 *
 *	- Pointer to \c dest
 */
char *strncpy(char *dest, const char *src, size_t n);

/**
 * @fn size_t strlen(const char *s)
 * @brief Calculate the length of a string
 * @param s String
 * @return
 *
 *	- Lenght of string
 */
size_t strlen(const char *s);

/**
 * @fn size_t strnlen(const char *s, size_t maxlen)
 * @brief Calculate the length of a string
 * @param s      String
 * @param maxlen Maximum length
 * @return
 *
 *	- Lenght of string or \c n
 */
size_t strnlen(const char *s, size_t maxlen);

/**
 * @fn char *strchr(const char *s, int c)
 * @brief Locate character in string
 * @param s String
 * @param c Character
 * @return
 *
 *	- Pointer to character - Success
 *	-                 NULL - Failure
 */
char *strchr(const char *s, int c);

/**
 * @fn const char *strerror(int errnum)
 * @brief Return string describing error number
 * @param errnum Error number
 * @return
 *
 *	- Pointer to error description - Success
 *	-                         NULL - Failure
 */
char *strerror(int errnum);

#endif /* ifndef _INCLUDE_CSTRING_H_ */
