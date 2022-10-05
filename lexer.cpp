#include <cassert>

#include "lexer.h"


namespace {

bool is_whitespace(char ch) {
  return (ch <= ' ' ) &&
         (ch != '\0') &&
         (ch != '\n');
}

bool is_hash(char ch) {
  return ch == '#';
}

bool is_eol(char ch) {
  return ch == '\n';
}

bool is_alpha(char ch) {
  return (ch >= 'a' && ch <= 'z') ||
         (ch >= 'A' && ch <= 'Z') ||
         (ch == '_');
}

bool is_number(char ch) {
  return ch >= '0' && ch <= '9';
}

bool is_eof(char ch) {
  return ch == '\0';
}

bool is_quote(char ch) {
  return ch == '"';
}

const char* skip_whitespace(const char* ptr) {
  for (; is_whitespace(*ptr); ++ptr);
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

void token_t::_raise_keyword() {
  assert(type == token_symbol);

#define CHECK(STR, TOK) \
  if (strmatch(STR)) { type = TOK; break; };

  switch (*ptr) {
  case 'a': CHECK("and",      token_and);      break;
  case 'c': CHECK("continue", token_continue); break;
  case 'e': CHECK("else",     token_else);
            CHECK("end",      token_end);      break;
  case 'g': CHECK("goto",     token_goto);     break;
  case 'i': CHECK("if",       token_if);       break;
  case 'f': CHECK("for",      token_for);
            CHECK("function", token_function); break;
  case 'n': CHECK("not",      token_not);      break;
  case 'o': CHECK("or",       token_or);       break;
  case 'r': CHECK("return",   token_return);   break;
  case 'v': CHECK("var",      token_var);      break;
  case 'w': CHECK("while",    token_while);    break;
  }

#undef CHECK
}

void lexer_t::state_t::advance(token_t& token) {

  ptr = skip_whitespace(ptr);
  token.ptr  = ptr;
  token.line = line;

  char ch = *ptr;

  if (is_hash(ch)) {
    for (; !(is_eol(ch) || is_eof(ch)); ch = nextChar());
    token.type = token_eol;
    ptr += is_eol(ch) ? 1 : 0;
    token.end = ptr;
    return;
  }

  if (is_number(ch)) {
    for (; is_number(ch); ch = nextChar());
    token.type = token_number;

    token.end = ptr;
    return;
  }

  if (is_alpha(ch)) {
    for (; is_alpha(ch) || is_number(ch); ch = nextChar());
    token.type = token_symbol;
    token.end = ptr;
    token._raise_keyword();
    return;
  }

  if (is_quote(ch)) {
    ch = nextChar();
    for (; !is_quote(ch) && !is_eof(ch); ch = nextChar());
    token.type = token_string;

    token.end = ++ptr;
    return;
  }

  switch (ch) {
  case '\0': token.type = token_eof;     break;
  case '\n': token.type = token_eol;     break;
  case '(':  token.type = token_lparen;  break;
  case ')':  token.type = token_rparen;  break;
  case '[':  token.type = token_lbrace;  break;
  case ']':  token.type = token_rbrace;  break;
  case '.':  token.type = token_dot;     break;
  case ',':  token.type = token_comma;   break;
  case '+':  token.type = token_plus;    break;
  case '-':  token.type = token_minus;   break;
  case '*':  token.type = token_mul;     break;
  case '/':  token.type = token_div;     break;
  case '%':  token.type = token_mod;     break;
  case ':':  token.type = token_colon;   break;
  default:   token.type = token_unknown; break;
  }

  nextChar();
  token.end = ptr;
}

char lexer_t::state_t::nextChar() {
  ptr  += (*ptr == '\0') ? 0 : 1;
  line += (*ptr == '\n') ? 1 : 0;
  return *ptr;
}
