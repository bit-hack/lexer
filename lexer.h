#pragma once
#include <cstdint>

enum token_type_t {
  token_unknown,
  token_eof,     // '\0'
  token_eol,     // '\n'

  token_symbol,
  token_number,
  token_string,  // "..."

  token_lparen,  // (
  token_rparen,  // )
  token_lbrace,  // [
  token_rbrace,  // ]
  token_dot,     // .
  token_comma,   // ,
  token_plus,    // +
  token_minus,   // -
  token_mul,     // *
  token_div,     // /
  token_mod,     // %
  token_colon,   // :

  token_and,
  token_continue,
  token_else,
  token_end,
  token_for,
  token_function,
  token_goto,
  token_if,
  token_not,
  token_or,
  token_return,
  token_var,
  token_while,
};

struct token_t {
  const char*  ptr;
  const char*  end;
  token_type_t type;
  uint32_t     line;

  bool strmatch(const char*) const;

  size_t length() const {
    return end - ptr;
  }

  void _raise_keyword();
};

struct lexer_t {

  lexer_t(const char* start, const char* end)
    : state(state_t{ start, end, 1 })
  {
  }

  bool found(token_type_t type) {
    state_t s = state;
    token_t t;
    s.advance(t);
    if (t.type == type) {
      state = s;
      return true;
    }
    return false;
  }

  void next(token_t& token) {
    state.advance(token);
  }

  void peek(token_t& token) {
    state_t s = state;
    s.advance(token);
  }

protected:
  struct state_t {
    const char* ptr;
    const char* end;
    uint32_t line;

    char nextChar();

    void advance(token_t& token);
  };

  state_t state;
};
