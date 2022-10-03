#include <cassert>

#include "parse.h"


namespace {

bool isWhitespace(char ch) {
  return (ch <= ' ' ) &&
         (ch != '\0') &&
         (ch != '\n');
}

bool isHash(char ch) {
  return ch == '#';
}

bool isEOL(char ch) {
  return ch == '\n';
}

bool isAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') ||
         (ch >= 'A' && ch <= 'Z') ||
         (ch == '_');
}

bool isNumber(char ch) {
  return ch >= '0' && ch <= '9';
}

bool isEOF(char ch) {
  return ch == '\0';
}

bool isQuote(char ch) {
  return ch == '"';
}

const char* skipWhitespace(const char* ptr) {
  for (; isWhitespace(*ptr); ++ptr);
  return ptr;
}

}  // namespace {}

bool token_t::strmatch(const char* x) const {
  const char* y = ptr;
  for (; ; ++x, ++y) {
    if (*x == '\0' && y == end) {
      return true;
    }
    if (*y != *x) {
      return false;
    }
  }
}

void token_t::_raiseKeyword() {
  assert(type == tokenSymbol);

#define CHECK(STR, TOK) \
  if (strmatch(STR)) { type = TOK; break; };

  switch (*ptr) {
  case 'a': CHECK("and",      tokenAnd);      break;
  case 'e': CHECK("else",     tokenElse);
            CHECK("end",      tokenEnd);      break;
  case 'i': CHECK("if",       tokenIf);       break;
  case 'f': CHECK("for",      tokenFor);
            CHECK("function", tokenFunction); break;
  case 'n': CHECK("not",      tokenNot);      break;
  case 'o': CHECK("or",       tokenOr);       break;
  case 'v': CHECK("var",      tokenVar);      break;
  case 'w': CHECK("while",    tokenWhile);    break;
  }

#undef CHECK
}

void parser_t::state_t::advance(token_t& token) {

  ptr = skipWhitespace(ptr);
  token.ptr  = ptr;
  token.line = line;

  char ch = *ptr;

  if (isHash(ch)) {
    for (; !(isEOL(ch) || isEOF(ch)); ch = nextChar());
    token.type = tokenEOL;
    ptr += isEOL(ch) ? 1 : 0;
    token.end = ptr;
    return;
  }

  if (isNumber(ch)) {
    for (; isNumber(ch); ch = nextChar());
    token.type = tokenNumber;

    token.end = ptr;
    return;
  }

  if (isAlpha(ch)) {
    for (; isAlpha(ch) || isNumber(ch); ch = nextChar());
    token.type = tokenSymbol;
    token.end = ptr;
    token._raiseKeyword();
    return;
  }

  if (isQuote(ch)) {
    ch = nextChar();
    for (; !isQuote(ch) && !isEOF(ch); ch = nextChar());
    token.type = tokenString;

    token.end = ++ptr;
    return;
  }

  switch (ch) {
  case '\0': token.type = tokenEOF;     break;
  case '\n': token.type = tokenEOL;     break;
  case '(':  token.type = tokenLParen;  break;
  case ')':  token.type = tokenRParen;  break;
  case '[':  token.type = tokenLBrace;  break;
  case ']':  token.type = tokenRBrace;  break;
  case '.':  token.type = tokenDot;     break;
  case ',':  token.type = tokenComma;   break;
  case '+':  token.type = tokenPlus;    break;
  case '-':  token.type = tokenMinus;   break;
  case '*':  token.type = tokenMul;     break;
  case '/':  token.type = tokenDiv;     break;
  case '%':  token.type = tokenMod;     break;
  default:   token.type = tokenUnknown; break;
  }

  nextChar();
  token.end = ptr;
}

char parser_t::state_t::nextChar() {
  ptr  += (*ptr == '\0') ? 0 : 1;
  line += (*ptr == '\n') ? 1 : 0;
  return *ptr;
}
