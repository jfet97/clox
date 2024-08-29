#include <stdlib.h>

#include "memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if(newSize == 0) {
    free(pointer);
    return NULL;
  }

  // it behaves like malloc when oldSize is 0
  void* result = realloc(pointer, newSize);
  if(result == NULL) exit(1);
  return result;
}