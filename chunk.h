#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// the OpCode enum is a list of all the bytecodes that the VM can execute
typedef enum {
  OP_RETURN, // return from the current function
  OP_CONSTANT, // OP_CONSTANT [idx] loads the constant value at index idx from the constant pool
} OpCode;

// it's a dynamic array
// to store some other data along with the instructions
typedef struct {
  int count;
  int capacity;
  uint8_t* code;
  int* lines;
  ValueArray constants;
} Chunk;


// initialize a new chunk
void initChunk(Chunk* chunk);

// append a byte to the end of the chunk
void writeChunk(Chunk* chunk, uint8_t byte, int line);

// free the memory used by the chunk
void freeChunk(Chunk* chunk);

// add a constant to the chunk
int addConstant(Chunk* chunk, Value value);

#endif