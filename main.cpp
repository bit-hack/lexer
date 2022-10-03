#include <string.h>
#include <stdio.h>

#include "lexer.h"


void test(const char *s) {
  size_t len = strlen(s);
  lexer_t lexer{ s, s + len };
  
  printf("------------------------------------------------\n");

  token_t token;
  do {

    lexer.next(token);

    size_t len = token.length();
    const char* str = token.ptr;

    printf("%2u ", token.type);

    switch (token.type) {
    case tokenEOF: printf("EOF\n"); break;
    case tokenEOL: printf("EOL\n"); break;
    default:
      printf("'%.*s'\n", len, str);
    }

  } while (token.type != tokenEOF);

}

int main(int argc, char** args) {

  test("");
  test("#");
  test("a");
  test("a, b,c");
  test("function test(a, b)");
  test("\"string\"");
  test("\"\"");
  test("a\nb");
  test("a # test");
  test("a # comment \nb");
  test("a 1234 b");
  test("\n\na");
  test("a123");
  test("1234asdf");
  test("if");

  return 0;
}
