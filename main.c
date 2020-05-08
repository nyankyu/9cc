#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ast.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "引数の数が正しくありません。１つの引数が必要です。\n");
    return (1);
  }

  tokenize(argv[1]);
  Node *node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");

  return (0);
}
