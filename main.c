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
  printf("main:\n");

  // prologue
  // stack size 8 * 26 = 208
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  for (int i = 0; g_code[i]; i++) {
    gen(g_code[i]);
    printf("  pop rax\n");
  }

  // epilogue
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");

  return (0);
}
