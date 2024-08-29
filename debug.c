#include <stdio.h>

#include "debug.h"
#include "value.h"

// access to static functions is restricted to the file where they are declared
static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
  uint8_t idx = chunk->code[offset + 1];
  printf("%-16s %4d '", name, idx);
  printValue(chunk->constants.values[idx]);
  printf("'\n");
  return offset + 2;
}

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);
  for(int offset = 0; offset < chunk->count;) {
    // instructions may have different sizes
    offset = disassembleInstruction(chunk, offset);
  }
}

int disassembleInstruction(Chunk* chunk, int offset) {
  // print the offset of the instruction
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];
  switch(instruction) {
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}
