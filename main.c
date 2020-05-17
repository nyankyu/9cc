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

  puts("#====== tokenize =======");
  tokenize(argv[1]);
  puts("#====== made AST =======");
  program();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");

  gen_function();

  return (0);
}
