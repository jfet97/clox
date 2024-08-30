#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define STACK_MAX 1024

typedef struct {
  Chunk* chunk;
  // instruction pointer (byte pointer)
  // it always points to the instruction (byte) about to be executed (read)
  uint8_t* ip;

  // stack based VM
  Value stack[STACK_MAX];
  // it's the array element just past the element containing the top value on the stack
  Value* stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();

#endif