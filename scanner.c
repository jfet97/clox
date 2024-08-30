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

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;

  return token; // by copy
}

static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;

  return token; // by copy
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

// return current and consume it
static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

// consume current only if current == expected
static bool match(char expected) {
  if(isAtEnd()) return false;
  if(*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

// return current
static char peek() {
  return *scanner.current;
}

// return next
static char peekNext() {
  if(isAtEnd()) return '\0';
  return scanner.current[1];
}

static void skipWhitespace() {
  while(true) {
    char c = peek();
    switch(c) {
      case ' ':
      case '\r':
      case '\t':
      case '\n': {
        if(c == '\n') scanner.line++;
        advance();
        break;
      }
      case '/': {
        if(peekNext() == '/') {
          // a comment goes until the end of the line
          // use peek to avoid consuming the newline, so that the line count will be incremented
          // on the next turn of the outer loop
          while(peek() != '\n' && !isAtEnd()) advance();
        } else {
          return;
        }
      }

      default:
        return;
    }
  }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
  // have we consumed enough characters to match the keyword?
  // if so, check if the characters match the keyword
  if(scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  } else {
    return TOKEN_IDENTIFIER;
  }
}

static TokenType identifierType() {
  // scanner.current is pointing to the character after the last character of the identifier
  switch (scanner.start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    case 'f': {
      // we consumed at least two characters
      if(scanner.current - scanner.start > 1) {
        switch(scanner.start[1]) {
          case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
        }
      }
      break;
    }
    case 't': {
      // we consumed at least two characters
      if(scanner.current - scanner.start > 1) {
        switch(scanner.start[1]) {
          case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
          case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
    }
  }


  return TOKEN_IDENTIFIER;
}

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c == '_';
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static Token string() {
  while(peek() != '"' && !isAtEnd()) {
    if(peek() == '\n') scanner.line++;
    advance();
  }

  if(isAtEnd()) return errorToken("Unterminated string.");

  // consume the closing "
  advance();
  return makeToken(TOKEN_STRING);
}

static Token number() {
  while (isDigit(peek())) advance();

  // look for a fractional part
  if(peek() == '.' && isDigit(peekNext())) {
    // consume the .
    advance();
    // and the following digits
    while(isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static Token identifier() {
  // the first is already checked before calling this function, and it's an alpha
  // we support digits in identifiers, but they can't be the first character
  while(isAlpha(peek()) || isDigit(peek())) advance();

  return makeToken(identifierType());
}

Token scanToken() {
  skipWhitespace();

  // each call to this function scans a complete token, so know we are
  // at the beginning of a new token when we enter the function
  scanner.start = scanner.current;

  if(isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();
  if(isAlpha(c)) return identifier();
  if(isDigit(c)) return number();

  switch(c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"': return string();
  }

  return errorToken("Unexpected character.");
}