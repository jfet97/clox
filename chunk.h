#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"

// the OpCode enum is a list of all the bytecodes that the VM can execute
typedef enum {
  OP_RETURN, // return from the current function
} OpCode;

// it's a dynamic array
// to store some other data along with the instructions
typedef struct {
  int count;
  int capacity;
  uint8_t* code;
} Chunk;


// initialize a new chunk
void initChunk(Chunk* chunk);

// append a byte to the end of the chunk
void writeChunk(Chunk* chunk, uint8_t byte);

// free the memory used by the chunk
void freeChunk(Chunk* chunk);

#endif