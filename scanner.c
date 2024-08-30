#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  // the beginning of the current lexeme
  const char* start;
  // the current character being looked at
  const char* current;
  int line;
} Scanner;

// just a single instance for simplicity
Scanner scanner;

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}