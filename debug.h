#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

// disassemble all the instructions in a chunk
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif