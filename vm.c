#include <stdio.h>

#include "common.h"
#include "vm.h"
#include "value.h"
#include "debug.h"

// just a static instance of the VM
VM vm;

static void resetStack() {
  // for efficiency, we don't clear the stack, we just reset the stackTop pointer
  // because of the semantics of the stackTop pointer, the stack is empty when stackTop points to the first element
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

static InterpretResult run() {
  #define READ_BYTE() (*vm.ip++)
  #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  while(true) {
    #ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for(Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    int offset = vm.ip - vm.chunk->code;
    disassembleInstruction(vm.chunk, offset);
    #endif


    uint8_t instruction;
    switch(instruction = READ_BYTE()) {
      case OP_RETURN:
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
    }
  }

  #undef READ_BYTE
  #undef READ_CONSTANT
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  // moving the stackTop pointer is enough
  vm.stackTop--;
  return *vm.stackTop;
}