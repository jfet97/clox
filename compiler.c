#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

Parser parser;

static void errorAtCurrent(const char* message) {
  errorAt(&parser.current, message);
}

static void error(const char* message) {
  errorAt(&parser.previous, message);
}

static void errorAt(Token* token, const char* message) {
  // suppress errors after the first one (will be cleared at synchronization points)
  if(parser.panicMode) return;
  parser.panicMode = true;

  fprintf(stderr, "[line %d] Error", token->line);

  if(token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if(token->type == TOKEN_ERROR) {
    // Nothing
  } else {
    fprintf(stderr, "at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

static void advance() {
  parser.previous = parser.current;

  while(true) {
    parser.current = scanToken();
    if(parser.current.type != TOKEN_ERROR) break;

    // the scanner doesn't report lexical errors, it creates error tokens
    // and leaves it up to the parser to report them
    errorAtCurrent(parser.current.start);
  }
}

static void consume(TokenType type, const char* message) {
  // check if the current token is of the expected type before advancing
  if(parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

bool compile(const char* source, Chunk* chunk) {
  initScanner(source);

  parser.hadError = false;
  parser.panicMode = false;


  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  return !parser.hadError;
}