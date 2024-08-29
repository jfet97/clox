#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
  (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, count) \
  reallocate(pointer, sizeof(type) * (count), 0)

/* **
  * Manages the memory allocation: single point of control for all memory allocation
  * to ease the development of a garbage collector
  *
  * oldSize = 0 && newSize > 0: allocate a new block of memory
  * oldSize > 0 && newSize = 0: free the block of memory
  * oldSize > 0 && newSize < oldSize: shrink the block of memory
  * oldSize > 0 && newSize > oldSize: grow the block of memory
*/
void* reallocate(void* pinter, size_t oldSize, size_t newSize);

#endif