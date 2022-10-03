#pragma once
#include <cstdint>

enum token_type_t {
  tokenUnknown,
  tokenEOF,
  tokenEOL,

  tokenSymbol,
  tokenNumber,
  tokenString,  // "..."

  tokenLParen,  // (
  tokenRParen,  // )
  tokenLBrace,  // [
  tokenRBrace,  // ]
  tokenDot,     // .
  tokenComma,   // ,
  tokenPlus,    // +
  tokenMinus,   // -
  tokenMul,     // *
  tokenDiv,     // /
  tokenMod,     // %

  tokenAnd,
  tokenElse,
  tokenEnd,
  tokenIf,
  tokenFor,
  tokenFunction,
  tokenNot,
  tokenOr,
  tokenVar,
  tokenWhile,
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

  void _raiseKeyword();
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
