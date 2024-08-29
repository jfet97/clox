#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

// something like a constant pool for values
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

// initialize a new constant pool
void initValueArray(ValueArray* array);

// append a value to the end of the pool
void writeValueArray(ValueArray* array, Value value);

// free the memory used by the pool
void freeValueArray(ValueArray* array);

#endif