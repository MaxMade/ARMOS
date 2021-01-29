#ifndef _INC_NEW_H_
#define _INC_NEW_H_

#include <cstddef.h>
#include <cstdlib.h>

/**
 * @file new.h
 * @brief Dynamic Memory Management
 */

void* operator new(size_t size);

void* operator new[](size_t size);

void operator delete(void* p);

void operator delete[](void* p);

void operator delete(void* p, size_t size);

void operator delete [](void* p, size_t size);

#endif /* ifndef _INC_NEW_H_ */
