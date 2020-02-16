#include <stdio.h>
#include <stdlib.h>
#include "token.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "引数の数が正しくありません。１つの引数が必要です。\n");
    return (1);
  }

  tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  printf("  mov rax, %d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }
    if (consume('-')) {
      printf("  sub rax, %d\n", expect_number());
      continue;
    }
  }

  printf("  ret\n");
  return (0);
}
